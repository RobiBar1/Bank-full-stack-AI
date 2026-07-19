// src/services/dashboard.service.js

class DashboardService {
    constructor(userRepository, transactionRepository) {
        this.userRepository = userRepository;
        this.transactionRepository = transactionRepository;
    }

    /**
     * Aggregates the user balance and formatted transaction history.
     * @param {string|number} userId 
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

        // 3. Format the transactions array to match the React client interfaces perfectly
        const formattedTransactions = rawTransactions.map(tx => {
            const isSender = tx.sender_id === userId;

            return {
                id: tx.id,
                // Maps to frontend dynamic coloring logic ('transfer_out' vs 'transfer_in')
                type: isSender ? 'transfer_out' : 'transfer_in', 
                // Passed without sign prefix because frontend applies currency formatting and adds sign dynamically
                amount: tx.amount, 
                date: tx.created_at,
                description: isSender 
                    ? `Transfer to ${tx.receiver_email}` 
                    : `Transfer from ${tx.sender_email}`
            };
        });

        return {
            balance: user.balance,
            transactions: formattedTransactions
        };
    }
}

module.exports = DashboardService;