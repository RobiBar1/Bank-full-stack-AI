// src/validations/transaction.schema.js
const { z } = require('zod');

const transferSchema = z.object({
    receiverEmail: z.string()
        .email('Invalid receiver email format'),
    amount: z.number({ required_error: "Amount is required", invalid_type_error: "Amount must be a number" })
        .positive('Transfer amount must be strictly greater than zero')
        // Prevents floating point precision issues beyond 2 decimal places in the API layer
        .multipleOf(0.01, 'Amount cannot have more than 2 decimal places') 
});

module.exports = {
    transferSchema
};