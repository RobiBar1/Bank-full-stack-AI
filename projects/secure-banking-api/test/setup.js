// test/setup.js
const { pool } = require('../src/config/db');
const { initializeDatabaseSchema } = require('../src/scripts/initTables');

/**
 * Runs once before any test suites start executing.
 * Connects to the DB and ensures the schema is built.
 */
beforeAll(async () => {
    if (process.env.NODE_ENV !== 'test') {
        throw new Error('Tests must be run with NODE_ENV=test');
    }
    await initializeDatabaseSchema();
});

/**
 * Runs after every individual test block (it/test).
 * Truncates all tables and cascades to wipe related records, ensuring total test isolation.
 */
afterEach(async () => {
    try {
        await pool.query('TRUNCATE TABLE transactions, users CASCADE;');
    } catch (error) {
        console.error('[Test Setup] Failed to truncate tables:', error.message);
    }
});

/**
 * Runs once after all test suites have finished executing.
 * Closes the database pool so Jest can exit cleanly without hanging.
 */
afterAll(async () => {
    await pool.end();
});