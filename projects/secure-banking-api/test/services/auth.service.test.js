// test/services/auth.service.test.js
const AuthService = require('../../src/services/auth.service');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');

jest.mock('bcryptjs');
jest.mock('jsonwebtoken');

describe('AuthService Unit Tests', () => {
    let mockUserRepo, mockSmsService, authService;

    beforeEach(() => {
        mockUserRepo = {
            findByEmail: jest.fn(),
            create: jest.fn(),
            markAsVerified: jest.fn()
        };
        mockSmsService = {
            sendOTP: jest.fn()
        };
        authService = new AuthService(mockUserRepo, mockSmsService);
        jest.clearAllMocks();
    });

    describe('registerUser', () => {
        it('should successfully register, hash password, create user, and trigger SMS dispatch', async () => {
            mockUserRepo.findByEmail.mockResolvedValue(null);
            bcrypt.genSalt.mockResolvedValue('salt-string');
            bcrypt.hash.mockResolvedValue('hashed-password');
            
            const mockUserRecord = { id: 'user-uuid', email: 'new@bank.com', balance: '5000.00' };
            mockUserRepo.create.mockResolvedValue(mockUserRecord);

            const result = await authService.registerUser('new@bank.com', '+1234567890', 'PlainPass123!');

            expect(mockUserRepo.findByEmail).toHaveBeenCalledWith('new@bank.com');
            expect(bcrypt.hash).toHaveBeenCalledWith('PlainPass123!', 'salt-string');
            expect(mockUserRepo.create).toHaveBeenCalledWith(
                expect.objectContaining({
                    email: 'new@bank.com',
                    password_hash: 'hashed-password',
                    phone: '+1234567890',
                    is_verified: false,
                    otp_code: expect.stringMatching(/^\d{6}$/)
                })
            );
            expect(mockSmsService.sendOTP).toHaveBeenCalledWith('+1234567890', expect.any(String));
            expect(result).toEqual(mockUserRecord);
        });

        it('should throw error if email already exists in system', async () => {
            mockUserRepo.findByEmail.mockResolvedValue({ id: 'existing-id' });

            await expect(authService.registerUser('new@bank.com', '+1234567890', 'PlainPass123!'))
                .rejects.toThrow('Email is already registered.');
            expect(mockUserRepo.create).not.toHaveBeenCalled();
        });
    });

    describe('verifyOTP', () => {
        it('should mark user as verified when accurate OTP is supplied', async () => {
            const mockUser = { id: 'uuid-1', is_verified: false, otp_code: '123456' };
            mockUserRepo.findByEmail.mockResolvedValue(mockUser);

            const result = await authService.verifyOTP('test@bank.com', '123456');

            expect(mockUserRepo.markAsVerified).toHaveBeenCalledWith('uuid-1');
            expect(result).toBe(true);
        });

        it('should fail verification if database record has user marked as verified already', async () => {
            mockUserRepo.findByEmail.mockResolvedValue({ id: 'uuid-1', is_verified: true });

            await expect(authService.verifyOTP('test@bank.com', '123456'))
                .rejects.toThrow('User is already verified.');
        });

        it('should fail verification if OTP does not match stored token', async () => {
            mockUserRepo.findByEmail.mockResolvedValue({ id: 'uuid-1', is_verified: false, otp_code: '111111' });

            await expect(authService.verifyOTP('test@bank.com', '222222'))
                .rejects.toThrow('Invalid OTP.');
        });
    });

    describe('login', () => {
        it('should verify password hash, generate, and return a signed jwt string', async () => {
            const mockUser = { id: 'uuid-1', email: 'test@bank.com', password_hash: 'hash-abc', is_verified: true };
            mockUserRepo.findByEmail.mockResolvedValue(mockUser);
            bcrypt.compare.mockResolvedValue(true);
            jwt.sign.mockReturnValue('jwt-token-string');

            const result = await authService.login('test@bank.com', 'PlainPass123!');

            expect(bcrypt.compare).toHaveBeenCalledWith('PlainPass123!', 'hash-abc');
            expect(jwt.sign).toHaveBeenCalledWith(
                { id: 'uuid-1', email: 'test@bank.com' },
                process.env.JWT_SECRET,
                { expiresIn: '1h' }
            );
            expect(result).toBe('jwt-token-string');
        });

        it('should throw explicit error if account is unverified', async () => {
            mockUserRepo.findByEmail.mockResolvedValue({ id: 'uuid-1', is_verified: false });

            await expect(authService.login('test@bank.com', 'any'))
                .rejects.toThrow('Account not verified. Please verify OTP first.');
        });
    });
});