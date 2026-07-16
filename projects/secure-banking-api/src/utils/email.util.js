const nodemailer = require('nodemailer');

class EmailService {
    constructor() {
        // Configure the Gmail SMTP connection
        this.transporter = nodemailer.createTransport({
            service: 'gmail',
            auth: {
                user: process.env.EMAIL_USER,
                pass: process.env.EMAIL_PASS
            }
        });
    }

    async sendOTP(toEmail, otpCode) {
        // Bypass entirely during automated Jest tests to prevent hanging
        if (process.env.NODE_ENV === 'test') {
            return; 
        }

        const mailOptions = {
            from: `"Secure Banking API" <${process.env.EMAIL_USER}>`,
            to: toEmail,
            subject: 'Your Security OTP Code',
            text: `Your secure banking OTP is: ${otpCode}`,
            html: `<h2>Secure Banking API</h2>
                   <p>Your verification code is: <strong>${otpCode}</strong></p>
                   <p>Please do not share this code with anyone.</p>`
        };

        await this.transporter.sendMail(mailOptions);
        console.log(`\n✉️  [EmailService] Real OTP sent to: ${toEmail}\n`);
    }
}

module.exports = EmailService;