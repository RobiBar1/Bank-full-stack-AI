// src/validations/user.schema.js
const { z } = require('zod');

const registerSchema = z.object({
    email: z.string()
        .email('Invalid email format'),
    password: z.string()
        .min(8, 'Password must be at least 8 characters long')
        .regex(/[A-Z]/, 'Password must contain at least one uppercase letter')
        .regex(/[0-9]/, 'Password must contain at least one number')
        .regex(/[\W_]/, 'Password must contain at least one special character'),
    phone: z.string()
        .min(10, 'Phone number is too short')
        .max(15, 'Phone number is too long')
        .regex(/^\+?[1-9]\d{1,14}$/, 'Invalid phone number format (E.164 required)')
        .optional(),
});

module.exports = {
    registerSchema
};