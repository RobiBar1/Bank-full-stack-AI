// src/scripts/initTables.js
const db = require('../config/db');
const bcrypt = require('bcrypt');

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
 * Seeds mock users into the database for testing real-time features.
 */
const seedMockUsers = async () => {
    try {
        const mockPassword = 'Password123!';
        const saltRounds = 10;
        const hashedPassword = await bcrypt.hash(mockPassword, saltRounds);

        const mockUsers = [
            {
                email: 'user1@test.com',
                password_hash: hashedPassword,
                phone: '+12345678901',
                balance: 1000.00,
                is_verified: true // Pre-verified to bypass OTP login blocks during tests
            },
            {
                email: 'user2@test.com',
                password_hash: hashedPassword,
                phone: '+12345678902',
                balance: 500.00,
                is_verified: true
            }
        ];

        console.log('[Database] Checking for testing seed data...');
        
        for (const user of mockUsers) {
            // ON CONFLICT DO NOTHING prevents row replication on repeated server restarts
            await db.query(`
                INSERT INTO users (email, password_hash, phone, balance, is_verified)
                VALUES ($1, $2, $3, $4, $5)
                ON CONFLICT (email) DO NOTHING;
            `, [user.email, user.password_hash, user.phone, user.balance, user.is_verified]);
        }
        
        console.log('[Database] Seeding phase complete. Mock users are validated and ready.');
    } catch (error) {
        console.error('[Database] Seeding mock users failed:', error.message);
    }
};

/**
 * Executes the DDL statements to ensure tables and columns exist.
 */
const initializeDatabaseSchema = async () => {
    try {
        await db.query(createTablesQuery);
        console.log('[Database] Schema initialization complete. Tables and columns verified.');
        
        // Execute the mock seeding step directly after schema verification
        await seedMockUsers();
    } catch (error) {
        console.error('[Database] Schema initialization failed:', error.message);
        process.exit(1);
    }
};

module.exports = {
    initializeDatabaseSchema
};