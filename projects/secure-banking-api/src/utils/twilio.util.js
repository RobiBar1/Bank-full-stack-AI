// src/utils/twilio.util.js
const twilio = require('twilio');

class SmsService {
    constructor() {
        // Detect if we are using the local development dummy credentials
        const isDummy = process.env.TWILIO_AUTH_TOKEN === 'dummy_auth_token_for_testing';
        this.isDummy = isDummy;
        
        // Only initialize the HTTP agent if we have real credentials and aren't running Jest
        if (process.env.NODE_ENV !== 'test' && !isDummy) {
            this.client = twilio(
                process.env.TWILIO_ACCOUNT_SID,
                process.env.TWILIO_AUTH_TOKEN,
                { autoRetry: true }
            );
        }
        
        this.fromPhone = process.env.TWILIO_PHONE_NUMBER;
    }

    async sendOTP(toPhone, otpCode) {
        // 1. Bypass entirely during automated Jest tests
        if (process.env.NODE_ENV === 'test') {
            return; 
        }

        // 2. Simulate the SMS during local development
        if (this.isDummy || !this.fromPhone || !process.env.TWILIO_ACCOUNT_SID) {
            console.log(`\n=========================================`);
            console.log(`📱 [Twilio Simulation] SMS sent to: ${toPhone}`);
            console.log(`🔑 OTP Code: ${otpCode}`);
            console.log(`=========================================\n`);
            return;
        }

        // 3. Send real SMS via Twilio API (Production)
        await this.client.messages.create({
            body: `Your secure banking OTP is: ${otpCode}`,
            from: this.fromPhone,
            to: toPhone
        });
    }
}

module.exports = SmsService;