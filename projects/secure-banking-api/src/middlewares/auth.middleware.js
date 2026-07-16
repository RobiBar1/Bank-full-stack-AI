// src/middlewares/auth.middleware.js
const jwt = require('jsonwebtoken');

/**
 * Express middleware to enforce JWT Bearer authentication.
 */
const authenticateJWT = (req, res, next) => {
    const authHeader = req.headers.authorization;

    // 1. Verify the header exists and follows the 'Bearer <token>' format
    if (!authHeader || !authHeader.startsWith('Bearer ')) {
        return res.status(401).json({
            error: {
                code: 401,
                message: 'Authentication required. Missing or invalid Bearer token.'
            }
        });
    }

    // 2. Extract the raw token string
    const token = authHeader.split(' ')[1];

    try {
        // 3. Synchronously verify the signature and expiration against the server secret
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        
        // 4. Inject the decoded payload into the request object
        req.user = decoded; 
        
        // 5. Pass control to the next middleware or route controller
        next();
        
    } catch (error) {
        // Handle specific JWT errors gracefully
        if (error.name === 'TokenExpiredError') {
            return res.status(401).json({
                error: { code: 401, message: 'Authentication token has expired. Please log in again.' }
            });
        }
        
        return res.status(403).json({
            error: { code: 403, message: 'Invalid authentication token.' }
        });
    }
};

module.exports = authenticateJWT;