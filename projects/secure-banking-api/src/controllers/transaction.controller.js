// src/controllers/transaction.controller.js

class TransactionController {
    constructor(transactionService) {
        this.service = transactionService;
    }

    /**
     * POST /api/transactions/transfer
     */
    processTransfer = async (req, res, next) => {
        try {
            // Extract sanitized data (receiver and amount) from the Zod validation
            const { receiverEmail, amount } = req.body;

            // ---> NEW: Extract the sender directly from the authenticated JWT token <---
            // This is injected by your authenticateJWT middleware, making it impossible to forge.
            const senderEmail = req.user.email;

            // Execute the business logic
            const transactionResult = await this.service.transferMoney(
                senderEmail, 
                receiverEmail, 
                amount
            );

            // Respond with the newly created transaction ledger record
            res.status(200).json({
                message: 'Transfer completed successfully',
                data: transactionResult
            });
            
        } catch (error) {
            // We override the default 500 status code for bad requests (like insufficient funds)
            if (error.message.includes('Transfer failed')) {
                error.statusCode = 400; 
            }
            
            // Forward to the global error middleware
            next(error);
        }
    };
}

module.exports = TransactionController;