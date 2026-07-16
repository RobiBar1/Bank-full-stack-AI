// test/services/dashboard.service.test.js
const DashboardService = require('../../src/services/dashboard.service');

describe('DashboardService Unit Tests', () => {
    let mockUserRepo, mockTxRepo, service;

    beforeEach(() => {
        mockUserRepo = { findById: jest.fn() };
        mockTxRepo = { findByUserId: jest.fn() };
        service = new DashboardService(mockUserRepo, mockTxRepo);
    });

    it('should retrieve balances and format transaction array ledger correctly with +/- identifiers', async () => {
        const requestingUserId = 'user-a';
        mockUserRepo.findById.mockResolvedValue({ id: requestingUserId, balance: '1250.50' });

        const mockTransactions = [
            { id: 'tx-1', sender_id: 'user-a', receiver_id: 'user-b', amount: '100.00', status: 'COMPLETED', created_at: '2026-07-15' },
            { id: 'tx-2', sender_id: 'user-c', receiver_id: 'user-a', amount: '45.00', status: 'COMPLETED', created_at: '2026-07-14' }
        ];
        mockTxRepo.findByUserId.mockResolvedValue(mockTransactions);

        const result = await service.getUserDashboard(requestingUserId);

        expect(result.current_balance).toBe('1250.50');
        expect(result.transaction_history).toHaveLength(2);
        
        // Assert Sender Details (DEBIT)
        expect(result.transaction_history[0]).toEqual({
            transaction_id: 'tx-1',
            type: 'DEBIT',
            amount: '-100.00',
            status: 'COMPLETED',
            counterparty_id: 'user-b',
            date: '2026-07-15'
        });

        // Assert Receiver Details (CREDIT)
        expect(result.transaction_history[1]).toEqual({
            transaction_id: 'tx-2',
            type: 'CREDIT',
            amount: '+45.00',
            status: 'COMPLETED',
            counterparty_id: 'user-c',
            date: '2026-07-14'
        });
    });

    it('should throw error if querying dashboard for non-existent user id', async () => {
        mockUserRepo.findById.mockResolvedValue(null);

        await expect(service.getUserDashboard('fake-id')).rejects.toThrow('User not found.');
    });
});