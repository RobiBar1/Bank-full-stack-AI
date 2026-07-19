// src/middlewares/validate.middleware.js
const { ZodError } = require('zod');

/**
 * Higher-order function that takes a Zod schema and returns an Express middleware.
 */
const validate = (schema) => (req, res, next) => {
    try {
        // Parse and overwrite req.body with stripped/sanitized values
        req.body = schema.parse(req.body);
        next();
    }
    catch (error)
    {
        if (error instanceof ZodError) {
            const issues = error.issues || error.errors || [];
            
            const formattedErrors = issues.map(err => ({    
                field: err.path.join('.'),
                message: err.message
            }));
            
            return res.status(400).json({
                error: {
                    code: 400,
                    message: 'Validation failed',
                    details: formattedErrors
                }
            });
        }
        next(error); // Pass non-Zod errors to the global error handler
    }
};

module.exports = validate;