// src/services/auth.service.js
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');

class AuthService {
    constructor(userRepository, emailService) {
        this.userRepository = userRepository;
        this.emailService = emailService;
    }

    async registerUser(email, phone, password) {
        // 1. Verify email uniqueness
        const existingUser = await this.userRepository.findByEmail(email);
        if (existingUser) {
            throw new Error('Email is already registered.');
        }

        // 2. Hash the password securely
        const salt = await bcrypt.genSalt(10);
        const passwordHash = await bcrypt.hash(password, salt);

        // 3. Generate a 6-digit OTP
        const otp = Math.floor(100000 + Math.random() * 900000).toString();

        // 4. Generate random initial balance between 1,000 and 10,000
        const randomBalance = (Math.random() * (10000 - 1000) + 1000).toFixed(2);

        // 5. Persist to database
        const newUser = await this.userRepository.create({
            email,
            password_hash: passwordHash,
            phone,
            balance: randomBalance,
            is_verified: false,
            otp_code: otp 
        });

        // 6. Dispatch Email
        await this.emailService.sendOTP(email, otp); // Fixed: using 'otp' instead of 'otpCode'

        return newUser;
    }

    /**
     * Verifies the provided OTP against the database record.
     * @param {string} email 
     * @param {string} otp 
     * @returns {Promise<boolean>}
     */
    async verifyOTP(email, otp) {
        const user = await this.userRepository.findByEmail(email);
        
        if (!user) {
            throw new Error('User not found.');
        }
        if (user.is_verified) {
            throw new Error('User is already verified.');
        }
        if (user.otp_code !== otp) {
            throw new Error('Invalid OTP.');
        }

        // OTP matches; update the database state
        await this.userRepository.markAsVerified(user.id);
        return true;
    }

    /**
     * Authenticates a user and issues a JWT if credentials are valid.
     * @param {string} email 
     * @param {string} password 
     * @returns {Promise<string>} The signed JWT token
     */
    async login(email, password) {
        const user = await this.userRepository.findByEmail(email);
        
        // Security best practice: Use generic error messages to prevent email enumeration
        if (!user) {
            throw new Error('Invalid credentials.');
        }
        if (!user.is_verified) {
            throw new Error('Account not verified. Please verify OTP first.');
        }

        // Compare the plain text password against the stored bcrypt hash
        const isMatch = await bcrypt.compare(password, user.password_hash);
        if (!isMatch) {
            throw new Error('Invalid credentials.');
        }

        // Generate the JWT payload containing strictly non-sensitive identification data
        const payload = {
            id: user.id,
            email: user.email
        };

        // Sign the token using the secret from .env, set to expire in 1 hour
        const token = jwt.sign(payload, process.env.JWT_SECRET, { expiresIn: '1h' });
        
        return token;
    }
}

module.exports = AuthService;