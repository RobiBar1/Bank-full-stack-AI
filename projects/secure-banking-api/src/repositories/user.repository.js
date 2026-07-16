// src/repositories/user.repository.js

class UserRepository {
    constructor(db) {
        this.db = db;
    }

    /**
     * Retrieves a user by their email address.
     * @param {string} email 
     * @param {object} [client] - Optional active PostgreSQL transaction client
     * @returns {Promise<object|undefined>}
     */
    async findByEmail(email, client = null) {
        const executor = client || this.db;
        
        const query = 'SELECT * FROM users WHERE email = $1';
        const result = await executor.query(query, [email]);
        
        return result.rows[0];
    }

    /**
     * Retrieves a user by their ID.
     * @param {string} id 
     * @param {object} [client]
     * @returns {Promise<object|undefined>}
     */
    async findById(id, client = null) {
        const executor = client || this.db;
        const query = 'SELECT * FROM users WHERE id = $1';
        const result = await executor.query(query, [id]);
        return result.rows[0];
    }

    /**
     * Creates a new user record in the database, including auth fields.
     * @param {object} userData 
     * @param {object} [client] - Optional active PostgreSQL transaction client
     * @returns {Promise<object>} The newly created user (excluding password hash)
     */
    async create(userData, client = null) {
        const executor = client || this.db;
        
        const query = `
            INSERT INTO users (email, password_hash, phone, balance, is_verified, otp_code)
            VALUES ($1, $2, $3, $4, $5, $6)
            RETURNING id, email, phone, balance, is_verified, created_at
        `;
        
        const values = [
            userData.email, 
            userData.password_hash, 
            userData.phone, 
            userData.balance || 0.00,
            userData.is_verified || false,
            userData.otp_code || null
        ];
        
        const result = await executor.query(query, values);
        return result.rows[0];
    }

    /**
     * Updates the user's verification status to true and clears the OTP.
     * @param {string} userId 
     * @param {object} [client]
     */
    async markAsVerified(userId, client = null) {
        const executor = client || this.db;
        const query = `
            UPDATE users 
            SET is_verified = TRUE, otp_code = NULL 
            WHERE id = $1 
            RETURNING id, email, is_verified;
        `;
        const result = await executor.query(query, [userId]);
        return result.rows[0];
    }
}

module.exports = UserRepository;