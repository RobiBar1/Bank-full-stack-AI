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
            // Note: PostgreSQL NUMERIC types are returned as strings in Node.js by default to prevent precision loss.
            if (parseFloat(sender.balance) < amount) {
                throw new Error('Insufficient funds.');
            }

            // 3. Deduct from sender
            // We use relative math in the database (balance = balance - X) to prevent concurrent race conditions
            await client.query(
                'UPDATE users SET balance = balance - $1 WHERE id = $2',
                [amount, sender.id]
            );

            // 4. Add to receiver
            await client.query(
                'UPDATE users SET balance = balance + $1 WHERE id = $2',
                [amount, receiver.id]
            );

            // 5. Audit the transaction in the ledger
            const transactionRecord = await this.transactionRepository.create({
                sender_id: sender.id,
                receiver_id: receiver.id,
                amount: amount,
                status: 'COMPLETED'
            }, client);

            // Commit all changes to the database permanently
            await client.query('COMMIT');

            return transactionRecord;

        } catch (error) {
            // Revert all changes in all tables if any step above failed
            await client.query('ROLLBACK');
            
            // Format a clear error to pass up to the controller
            const failureMessage = error.message || 'Unknown database error occurred.';
            throw new Error(`Transfer failed: ${failureMessage}`);
            
        } finally {
            // Guarantee the client is returned to the pool, even if the node process crashes inside the try block
            client.release();
        }
    }
}

module.exports = TransactionService;