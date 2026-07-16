// test/controllers/dashboard.controller.test.js
const DashboardController = require('../../src/controllers/dashboard.controller');

describe('DashboardController Unit Tests', () => {
    let mockReq, mockRes, mockNext, mockDashboardService, controller;

    beforeEach(() => {
        mockReq = {
            user: { id: 'user-uuid-999' } // Populated by authenticateJWT middleware
        };
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
        mockDashboardService = { getUserDashboard: jest.fn() };
        controller = new DashboardController(mockDashboardService);
    });

    it('should return 200 and formatted aggregation payload', async () => {
        const mockDashboardPayload = { current_balance: '120.00', transaction_history: [] };
        mockDashboardService.getUserDashboard.mockResolvedValue(mockDashboardPayload);

        await controller.getDashboard(mockReq, mockRes, mockNext);

        expect(mockDashboardService.getUserDashboard).toHaveBeenCalledWith('user-uuid-999');
        expect(mockRes.status).toHaveBeenCalledWith(200);
        expect(mockRes.json).toHaveBeenCalledWith({
            message: 'Dashboard data retrieved successfully',
            data: mockDashboardPayload
        });
    });

    it('should set statusCode to 404 if user balance record is not discovered', async () => {
        const missingUserError = new Error('User not found.');
        mockDashboardService.getUserDashboard.mockRejectedValue(missingUserError);

        await controller.getDashboard(mockReq, mockRes, mockNext);

        expect(missingUserError.statusCode).toBe(404);
        expect(mockNext).toHaveBeenCalledWith(missingUserError);
    });
});