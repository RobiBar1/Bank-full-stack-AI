// test/controllers/transaction.controller.test.js
const TransactionController = require('../../src/controllers/transaction.controller');

describe('TransactionController Unit Tests', () => {
    let mockReq, mockRes, mockNext, mockService;
    let controller;

    beforeEach(() => {
        mockReq = {
            body: { receiverEmail: 'b@test.com', amount: 50 },
            user: { email: 'a@test.com' } 
        };
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
        mockService = { transferMoney: jest.fn() };
        controller = new TransactionController(mockService);
    });

    it('should return 200 and data on successful transfer', async () => {
        const mockResult = { id: 'tx-123' };
        mockService.transferMoney.mockResolvedValue(mockResult);

        await controller.processTransfer(mockReq, mockRes, mockNext);

        expect(mockService.transferMoney).toHaveBeenCalledWith('a@test.com', 'b@test.com', 50);
        expect(mockRes.status).toHaveBeenCalledWith(200);
        expect(mockRes.json).toHaveBeenCalledWith({
            message: 'Transfer completed successfully',
            data: mockResult
        });
    });

    it('should set statusCode to 400 for bad business logic requests', async () => {
        const error = new Error('Transfer failed: Insufficient funds.');
        mockService.transferMoney.mockRejectedValue(error);

        await controller.processTransfer(mockReq, mockRes, mockNext);

        expect(error.statusCode).toBe(400);
        expect(mockNext).toHaveBeenCalledWith(error);
    });
});