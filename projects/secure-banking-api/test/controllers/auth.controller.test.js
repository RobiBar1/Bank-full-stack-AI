// test/controllers/auth.controller.test.js
const AuthController = require('../../src/controllers/auth.controller');

describe('AuthController Unit Tests', () => {
    let mockReq, mockRes, mockNext, mockAuthService, controller;

    beforeEach(() => {
        mockReq = {
            body: { email: 'test@bank.com', phone: '+123', password: 'Pass' }
        };
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
        mockAuthService = {
            registerUser: jest.fn()
        };
        controller = new AuthController(mockAuthService);
    });

    it('should return 201 and user data excluding sensitive properties (password, otp)', async () => {
        const mockRawReturnedUser = {
            id: 'uuid-123',
            email: 'test@bank.com',
            phone: '+123',
            balance: '5000.00',
            password_hash: 'secret-hash', // Should be removed
            otp_code: '999999',           // Should be removed
            is_verified: false,
            created_at: '2026-07-15T00:00:00.000Z'
        };
        mockAuthService.registerUser.mockResolvedValue(mockRawReturnedUser);

        await controller.register(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(201);
        expect(mockRes.json).toHaveBeenCalledWith({
            message: 'User registered successfully. OTP sent to phone.',
            data: {
                id: 'uuid-123',
                email: 'test@bank.com',
                phone: '+123',
                balance: '5000.00',
                is_verified: false,
                created_at: '2026-07-15T00:00:00.000Z'
            }
        });
    });

    it('should set error statusCode to 409 when service flags duplicate email', async () => {
        const duplicateError = new Error('Email is already registered.');
        mockAuthService.registerUser.mockRejectedValue(duplicateError);

        await controller.register(mockReq, mockRes, mockNext);

        expect(duplicateError.statusCode).toBe(409);
        expect(mockNext).toHaveBeenCalledWith(duplicateError);
    });
});