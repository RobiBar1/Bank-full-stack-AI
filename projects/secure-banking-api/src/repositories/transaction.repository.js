// src/repositories/transaction.repository.js

class TransactionRepository {
    constructor(db) {
        this.db = db;
    }

    /**
     * Creates a new transaction ledger record.
     * @param {object} transactionData 
     * @param {object} [client] - Optional active PostgreSQL transaction client
     * @returns {Promise<object>} The created transaction record
     */
    async create(transactionData, client = null) {
        const executor = client || this.db;
        
        const query = `
            INSERT INTO transactions (sender_id, receiver_id, amount, status)
            VALUES ($1, $2, $3, $4)
            RETURNING id, sender_id, receiver_id, amount, status, created_at
        `;
        
        const values = [
            transactionData.sender_id,
            transactionData.receiver_id,
            transactionData.amount,
            transactionData.status || 'COMPLETED'
        ];
        
        const result = await executor.query(query, values);
        return result.rows[0];
    }

    /**
     * Retrieves all transactions where the user is the sender or receiver,
     * including the email addresses of both parties.
     * @param {string} userId 
     * @param {object} [client]
     * @returns {Promise<Array>}
     */
    async findByUserId(userId, client = null) {
        const executor = client || this.db;
        const query = `
            SELECT 
                t.id, 
                t.sender_id, 
                t.receiver_id, 
                t.amount, 
                t.status, 
                t.created_at,
                sender.email AS sender_email,
                receiver.email AS receiver_email
            FROM transactions t
            LEFT JOIN users sender ON t.sender_id = sender.id
            LEFT JOIN users receiver ON t.receiver_id = receiver.id
            WHERE t.sender_id = $1 OR t.receiver_id = $1 
            ORDER BY t.created_at DESC
        `;
        const result = await executor.query(query, [userId]);
        return result.rows;
    }
}

module.exports = TransactionRepository;