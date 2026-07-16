// src/scripts/initTables.js
const db = require('../config/db');

const createTablesQuery = `
    CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

    -- 1. Create tables if this is a fresh database
    CREATE TABLE IF NOT EXISTS users (
        id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
        email VARCHAR(255) UNIQUE NOT NULL,
        password_hash VARCHAR(255) NOT NULL,
        phone VARCHAR(20),
        balance NUMERIC(15, 2) NOT NULL DEFAULT 0.00 CHECK (balance >= 0),
        created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
    );

    CREATE TABLE IF NOT EXISTS transactions (
        id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
        sender_id UUID REFERENCES users(id) ON DELETE RESTRICT,
        receiver_id UUID REFERENCES users(id) ON DELETE RESTRICT,
        amount NUMERIC(15, 2) NOT NULL CHECK (amount > 0),
        status VARCHAR(50) NOT NULL DEFAULT 'PENDING',
        created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
    );

    CREATE INDEX IF NOT EXISTS idx_transactions_sender ON transactions(sender_id);
    CREATE INDEX IF NOT EXISTS idx_transactions_receiver ON transactions(receiver_id);

    -- 2. Add the new authentication columns if the table already existed from Day 1/Day 2
    ALTER TABLE users ADD COLUMN IF NOT EXISTS is_verified BOOLEAN DEFAULT FALSE;
    ALTER TABLE users ADD COLUMN IF NOT EXISTS otp_code VARCHAR(6);
`;

/**
 * Executes the DDL statements to ensure tables and columns exist.
 */
const initializeDatabaseSchema = async () => {
    try {
        await db.query(createTablesQuery);
        console.log('[Database] Schema initialization complete. Tables and columns verified.');
    } catch (error) {
        console.error('[Database] Schema initialization failed:', error.message);
        process.exit(1);
    }
};

module.exports = {
    initializeDatabaseSchema
};