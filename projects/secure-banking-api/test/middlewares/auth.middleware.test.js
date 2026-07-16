// test/middlewares/auth.middleware.test.js
const jwt = require('jsonwebtoken');
const authenticateJWT = require('../../src/middlewares/auth.middleware');

jest.mock('jsonwebtoken');

describe('authenticateJWT Middleware Unit Tests', () => {
    let mockReq, mockRes, mockNext;

    beforeEach(() => {
        mockReq = { headers: {} };
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
        jest.clearAllMocks();
    });

    it('should call next() and attach decoded payload to req.user when token is valid', () => {
        mockReq.headers.authorization = 'Bearer valid-token-123';
        const mockDecoded = { id: 'user-uuid', email: 'test@bank.com' };
        jwt.verify.mockReturnValue(mockDecoded);

        authenticateJWT(mockReq, mockRes, mockNext);

        expect(jwt.verify).toHaveBeenCalledWith('valid-token-123', process.env.JWT_SECRET);
        expect(mockReq.user).toEqual(mockDecoded);
        expect(mockNext).toHaveBeenCalledTimes(1);
    });

    it('should return 401 when Authorization header is missing', () => {
        authenticateJWT(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(401);
        expect(mockRes.json).toHaveBeenCalledWith({
            error: {
                code: 401,
                message: 'Authentication required. Missing or invalid Bearer token.'
            }
        });
        expect(mockNext).not.toHaveBeenCalled();
    });

    it('should return 401 when token format is not Bearer', () => {
        mockReq.headers.authorization = 'Basic basic-credentials';

        authenticateJWT(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(401);
        expect(mockNext).not.toHaveBeenCalled();
    });

    it('should return 401 with explicit expired message when token has expired', () => {
        mockReq.headers.authorization = 'Bearer expired-token';
        const expiredError = new Error('jwt expired');
        expiredError.name = 'TokenExpiredError';
        jwt.verify.mockImplementation(() => { throw expiredError; });

        authenticateJWT(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(401);
        expect(mockRes.json).toHaveBeenCalledWith({
            error: {
                code: 401,
                message: 'Authentication token has expired. Please log in again.'
            }
        });
    });

    it('should return 403 when token is structurally invalid or signature is incorrect', () => {
        mockReq.headers.authorization = 'Bearer tampered-token';
        jwt.verify.mockImplementation(() => { throw new Error('invalid signature'); });

        authenticateJWT(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(403);
        expect(mockRes.json).toHaveBeenCalledWith({
            error: {
                code: 403,
                message: 'Invalid authentication token.'
            }
        });
    });
});