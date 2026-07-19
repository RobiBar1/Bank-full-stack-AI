// src/services/transaction.service.js

class TransactionService {
    constructor(dbPool, userRepository, transactionRepository) {
        this.dbPool = dbPool; // The shared pg pool
        this.userRepository = userRepository;
        this.transactionRepository = transactionRepository;
    }

    /**
     * Executes a secure, atomic money transfer between two users.
     * @param {string} senderEmail 
     * @param {string} receiverEmail 
     * @param {number} amount 
     */
    async transferMoney(senderEmail, receiverEmail, amount) {
        if (senderEmail === receiverEmail) {
            throw new Error('Sender and receiver cannot be the same account.');
        }

        // Acquire a dedicated client from the pool for this specific transaction
        const client = await this.dbPool.connect();

        try {
            // Initiate the SQL transaction
            await client.query('BEGIN');

            // 1. Validate both users exist using the shared transaction client
            const sender = await this.userRepository.findByEmail(senderEmail, client);
            const receiver = await this.userRepository.findByEmail(receiverEmail, client);

            if (!sender) throw new Error('Sender account not found.');
            if (!receiver) throw new Error('Receiver account not found.');

            // 2. Validate sufficient funds
            if (parseFloat(sender.balance) < amount) {
                throw new Error('Insufficient funds.');
            }

            // 3. Deduct from sender
            await client.query(
                'UPDATE users SET balance = balance - $1 WHERE id = $2',
                [amount, sender.id]
            );

            // 4. Add to receiver
            await client.query(
                'UPDATE users SET balance = balance + $1 WHERE id = $2',
                [amount, receiver.id]
            );

            // Fetch the exact updated balance of the receiver within the transaction isolation layer
            const balanceRes = await client.query(
                'SELECT balance FROM users WHERE id = $1',
                [receiver.id]
            );
            const newReceiverBalance = balanceRes.rows[0].balance;

            // 5. Audit the transaction in the ledger
            const transactionRecord = await this.transactionRepository.create({
                sender_id: sender.id,
                receiver_id: receiver.id,
                amount: amount,
                status: 'COMPLETED'
            }, client);

            // Commit all changes to the database permanently
            await client.query('COMMIT');

            // 6. Broadcast real-time event to the receiver
            try {
                // Dynamically required to prevent potential lifecycle ordering issues during app boot
                const { getIO } = require('../config/socket');
                const io = getIO();
                
                io.to(receiverEmail).emit('balance_updated', {
                    message: `You received ${amount} USD.`,
                    updatedBalance: newReceiverBalance,
                    transaction: {
                        id: transactionRecord.id,
                        type: 'transfer_in',
                        amount: amount,
                        date: transactionRecord.created_at || new Date().toISOString(),
                        description: `Transfer from ${senderEmail}`
                    }
                });
            } catch (socketError) {
                // Catch socket errors separately so a network failure doesn't crash an already committed DB transfer
                console.error('[Socket.IO Event Error] Real-time transmission failed:', socketError.message);
            }

            return transactionRecord;

        } catch (error) {
            // Revert all changes in all tables if any step above failed
            await client.query('ROLLBACK');
            
            const failureMessage = error.message || 'Unknown database error occurred.';
            throw new Error(`Transfer failed: ${failureMessage}`);
            
        } finally {
            // Guarantee the client is returned to the pool
            client.release();
        }
    }
}

module.exports = TransactionService;