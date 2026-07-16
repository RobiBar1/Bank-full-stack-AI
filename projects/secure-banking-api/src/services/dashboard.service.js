// src/services/dashboard.service.js

class DashboardService {
    constructor(userRepository, transactionRepository) {
        this.userRepository = userRepository;
        this.transactionRepository = transactionRepository;
    }

    /**
     * Aggregates the user balance and formatted transaction history.
     * @param {string} userId 
     * @returns {Promise<object>}
     */
    async getUserDashboard(userId) {
        // 1. Fetch user to get current balance
        const user = await this.userRepository.findById(userId);
        if (!user) {
            throw new Error('User not found.');
        }

        // 2. Fetch all related transactions
        const rawTransactions = await this.transactionRepository.findByUserId(userId);

        // 3. Format the transactions array
        const formattedTransactions = rawTransactions.map(tx => {
            // Determine if the requesting user sent or received the money
            const isSender = tx.sender_id === userId;
            
            // Format the amount string
            const sign = isSender ? '-' : '+';
            const formattedAmount = `${sign}${tx.amount}`;

            return {
                transaction_id: tx.id,
                type: isSender ? 'DEBIT' : 'CREDIT',
                amount: formattedAmount,
                status: tx.status,
                counterparty_id: isSender ? tx.receiver_id : tx.sender_id,
                date: tx.created_at
            };
        });

        return {
            current_balance: user.balance,
            transaction_history: formattedTransactions
        };
    }
}

module.exports = DashboardService;