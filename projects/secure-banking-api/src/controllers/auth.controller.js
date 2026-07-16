// src/controllers/auth.controller.js

class AuthController {
    constructor(authService) {
        this.authService = authService;
    }

    register = async (req, res, next) => {
        try {
            const { email, phone, password } = req.body;
            const user = await this.authService.registerUser(email, phone, password);

            const safeUser = {
                id: user.id,
                email: user.email,
                phone: user.phone,
                balance: user.balance,
                is_verified: user.is_verified,
                created_at: user.created_at
            };

            res.status(201).json({
                message: 'User registered successfully. OTP sent to phone.',
                data: safeUser
            });
        } catch (error) {
            if (error.message === 'Email is already registered.') {
                error.statusCode = 409;
            }
            next(error);
        }
    };

    verify = async (req, res, next) => {
        try {
            const { email, otp } = req.body;
            
            // Calls the service method we already wrote
            await this.authService.verifyOTP(email, otp);
            
            res.status(200).json({
                message: 'Account verified successfully. You can now log in.'
            });
        } catch (error) {
            // Map business logic errors to proper HTTP status codes
            if (error.message === 'Invalid OTP.' || error.message === 'User not found.') {
                error.statusCode = 400;
            } else if (error.message === 'User is already verified.') {
                error.statusCode = 409;
            }
            next(error);
        }
    };

    // ---> NEW: The missing login handler <---
    login = async (req, res, next) => {
        try {
            const { email, password } = req.body;
            const token = await this.authService.login(email, password);
            
            res.status(200).json({
                message: 'Login successful',
                token: token
            });
        } catch (error) {
            if (error.message === 'Invalid credentials.' || error.message.includes('Account not verified')) {
                error.statusCode = 401; 
            }
            next(error);
        }
    };
}

module.exports = AuthController;