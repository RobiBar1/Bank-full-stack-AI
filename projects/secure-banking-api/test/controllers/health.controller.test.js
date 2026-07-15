// test/controllers/health.controller.test.js
const HealthController = require('../../src/controllers/health.controller');

describe('HealthController Unit Tests', () => {
    let mockReq, mockRes, mockNext;

    beforeEach(() => {
        mockReq = {};
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
    });

    it('should return HTTP 200 when system metrics are OK', async () => {
        const mockMetrics = { status: 'OK', database_connected: true };
        const mockService = {
            getSystemMetrics: jest.fn().mockResolvedValue(mockMetrics)
        };
        const controller = new HealthController(mockService);

        await controller.checkHealth(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(200);
        expect(mockRes.json).toHaveBeenCalledWith(mockMetrics);
        expect(mockNext).not.toHaveBeenCalled();
    });

    it('should pass errors to the global error handler via next()', async () => {
        const mockError = new Error('Database timeout');
        const mockService = {
            getSystemMetrics: jest.fn().mockRejectedValue(mockError)
        };
        const controller = new HealthController(mockService);

        await controller.checkHealth(mockReq, mockRes, mockNext);

        expect(mockNext).toHaveBeenCalledWith(mockError);
        expect(mockRes.status).not.toHaveBeenCalled();
    });
});