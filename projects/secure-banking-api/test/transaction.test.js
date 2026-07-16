// test/transaction.test.js
const request = require('supertest');
const app = require('../src/server');
const { pool } = require('../src/config/db');

describe('Transaction API Integration Tests', () => {
    const sender = { email: 'sender@bank.com', phone: '+1234567890', password: 'SecurePassword123!' };
    const receiver = { email: 'receiver@bank.com', phone: '+1987654321', password: 'SecurePassword456!' };
    let senderToken;

    beforeEach(async () => {
        // 1. Register both users via the API
        await request(app).post('/api/auth/register').send(sender);
        await request(app).post('/api/auth/register').send(receiver);

        // 2. Programmatically verify them and set standard balances for predictable testing
        await pool.query(`
            UPDATE users 
            SET is_verified = TRUE, balance = 5000.00 
            WHERE email = $1;
        `, [sender.email]);

        await pool.query(`
            UPDATE users 
            SET is_verified = TRUE, balance = 1000.00 
            WHERE email = $1;
        `, [receiver.email]);

        // 3. Login the sender to extract the JWT
        const loginRes = await request(app)
            .post('/api/auth/login')
            .send({ email: sender.email, password: sender.password });
        
        senderToken = loginRes.body.token;
    });

    it('should successfully transfer funds and update database balances', async () => {
        const transferAmount = 500.50;

        // 1. Execute the secure transfer request
        const response = await request(app)
            .post('/api/transactions/transfer')
            .set('Authorization', `Bearer ${senderToken}`)
            .send({
                senderEmail: sender.email,
                receiverEmail: receiver.email,
                amount: transferAmount
            });

        // 2. Assert the HTTP response
        expect(response.status).toBe(200);
        expect(response.body.message).toBe('Transfer completed successfully');
        expect(response.body.data.amount).toBe(transferAmount.toFixed(2));

        // 3. Verify the atomic database updates occurred correctly
        const senderDbRecord = await pool.query('SELECT balance FROM users WHERE email = $1', [sender.email]);
        const receiverDbRecord = await pool.query('SELECT balance FROM users WHERE email = $1', [receiver.email]);

        // Sender started with 5000.00, sent 500.50 = 4499.50
        expect(parseFloat(senderDbRecord.rows[0].balance)).toBe(4499.50);
        
        // Receiver started with 1000.00, received 500.50 = 1500.50
        expect(parseFloat(receiverDbRecord.rows[0].balance)).toBe(1500.50);
    });

    it('should block the transfer and return 401 if JWT is missing', async () => {
        const response = await request(app)
            .post('/api/transactions/transfer')
            // Notice: .set('Authorization') is intentionally omitted
            .send({
                senderEmail: sender.email,
                receiverEmail: receiver.email,
                amount: 100
            });

        expect(response.status).toBe(401);
        expect(response.body.error.message).toMatch(/Missing or invalid Bearer token/);
    });
});