// src/middlewares/error.middleware.js

/**
 * Global Error Handler
 * Express recognizes this as an error-handling middleware specifically because it accepts 4 arguments.
 */
const errorHandler = (err, req, res, next) => {
    // 1. Log the internal error stack for debugging
    console.error(`[Error Boundary] ${err.name}: ${err.message}`);
    
    if (process.env.NODE_ENV !== 'production') {
        console.error(err.stack);
    }

    // 2. Normalize the HTTP status code (default to 500 for unhandled exceptions)
    const statusCode = err.statusCode || 500;

    // 3. Mask internal server errors from the client to prevent security leakage
    const isProduction = process.env.NODE_ENV === 'production';
    const message = (isProduction && statusCode === 500) 
        ? 'Internal Server Error' 
        : err.message;

    // 4. Return standard JSON payload
    res.status(statusCode).json({
        error: {
            code: statusCode,
            message: message
        }
    });
};

module.exports = errorHandler;