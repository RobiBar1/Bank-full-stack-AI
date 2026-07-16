// test/auth.test.js
const request = require('supertest');
const app = require('../src/server');
const { pool } = require('../src/config/db');

describe('Authentication API Integration Tests', () => {
    const testUser = {
        email: 'integration@bank.com',
        phone: '+1234567890',
        password: 'SecurePassword123!'
    };

    it('should successfully register a new user and return 201', async () => {
        const response = await request(app)
            .post('/api/auth/register')
            .send(testUser);

        expect(response.status).toBe(201);
        expect(response.body.message).toBe('User registered successfully. OTP sent to phone.');
        expect(response.body.data.email).toBe(testUser.email);
        expect(response.body.data.is_verified).toBe(false);
        expect(response.body.data.password_hash).toBeUndefined(); // Security check
    });

    it('should reject registration with a duplicate email and return 409', async () => {
        // Register the user the first time
        await request(app).post('/api/auth/register').send(testUser);

        // Attempt to register the exact same email again
        const response = await request(app)
            .post('/api/auth/register')
            .send(testUser);

        expect(response.status).toBe(409);
        expect(response.text).toContain('Email is already registered');
    });

    it('should successfully log in a verified user and return a JWT', async () => {
        // 1. Register the user
        await request(app).post('/api/auth/register').send(testUser);

        // 2. Manually verify the user in the database (simulating successful OTP verification)
        await pool.query('UPDATE users SET is_verified = TRUE WHERE email = $1', [testUser.email]);

        // 3. Attempt Login
        const response = await request(app)
            .post('/api/auth/login')
            .send({
                email: testUser.email,
                password: testUser.password
            });

        expect(response.status).toBe(200);
        expect(response.body.token).toBeDefined();
        
        // Ensure the token is a valid string format (Header.Payload.Signature)
        const tokenParts = response.body.token.split('.');
        expect(tokenParts.length).toBe(3);
    });
});