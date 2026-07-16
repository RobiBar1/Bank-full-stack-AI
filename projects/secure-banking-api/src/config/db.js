// src/config/db.js
const { Pool } = require('pg');

const pool = new Pool({
    connectionString: process.env.DATABASE_URL,
    max: 20,
    idleTimeoutMillis: 30000,
    connectionTimeoutMillis: 2000,
});

/**
 * Tests the PostgreSQL connection on application startup.
 */
const testConnection = async () => {
    try {
        const client = await pool.connect();
        console.log('[Database] Successfully connected to PostgreSQL.');
        client.release();
    } catch (error) {
        console.error('[Database] Connection failed:', error.message);
        process.exit(1);
    }
};

module.exports = {
    pool,
    query: (text, params) => pool.query(text, params),
    testConnection
};