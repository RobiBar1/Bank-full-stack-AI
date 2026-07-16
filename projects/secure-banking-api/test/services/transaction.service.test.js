const TransactionService = require('../../src/services/transaction.service');

describe('TransactionService Unit Tests', () => {
    let mockDbPool, mockClient, mockUserRepo, mockTransactionRepo;
    let transactionService;

    beforeEach(() => {
        // Mock the dedicated database client
        mockClient = {
            query: jest.fn().mockResolvedValue({}),
            release: jest.fn()
        };

        // Mock the connection pool
        mockDbPool = {
            connect: jest.fn().mockResolvedValue(mockClient)
        };

        mockUserRepo = { findByEmail: jest.fn() };
        mockTransactionRepo = { create: jest.fn() };

        transactionService = new TransactionService(mockDbPool, mockUserRepo, mockTransactionRepo);
    });

    it('should complete transfer and COMMIT when funds are sufficient', async () => {
        // Arrange
        mockUserRepo.findByEmail
            .mockResolvedValueOnce({ id: '1', balance: '100.00' }) // Sender
            .mockResolvedValueOnce({ id: '2', balance: '50.00' }); // Receiver
            
        const mockRecord = { id: 'tx-1', amount: 20 };
        mockTransactionRepo.create.mockResolvedValue(mockRecord);

        // Act
        const result = await transactionService.transferMoney('sender@test.com', 'receiver@test.com', 20);

        // Assert
        expect(mockClient.query).toHaveBeenCalledWith('BEGIN');
        expect(mockClient.query).toHaveBeenCalledWith(
            'UPDATE users SET balance = balance - $1 WHERE id = $2', [20, '1']
        );
        expect(mockClient.query).toHaveBeenCalledWith('COMMIT');
        expect(mockClient.release).toHaveBeenCalled();
        expect(result).toEqual(mockRecord);
    });

    it('should ROLLBACK and throw if sender has insufficient funds', async () => {
        // Arrange
        mockUserRepo.findByEmail
            .mockResolvedValueOnce({ id: '1', balance: '10.00' }) // Sender has 10
            .mockResolvedValueOnce({ id: '2', balance: '50.00' }); // Receiver

        // Act & Assert
        await expect(transactionService.transferMoney('sender@test.com', 'receiver@test.com', 20))
            .rejects.toThrow('Transfer failed: Insufficient funds.');

        expect(mockClient.query).toHaveBeenCalledWith('BEGIN');
        expect(mockClient.query).toHaveBeenCalledWith('ROLLBACK');
        expect(mockClient.query).not.toHaveBeenCalledWith('COMMIT');
        expect(mockClient.release).toHaveBeenCalled(); // Must always release
    });
});