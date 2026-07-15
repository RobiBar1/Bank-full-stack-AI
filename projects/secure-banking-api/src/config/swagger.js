// src/config/swagger.js
const swaggerJSDoc = require('swagger-jsdoc');

const options = {
    definition: {
        openapi: '3.0.0',
        info: {
            title: 'Secure Banking API',
            version: '1.0.0',
            description: 'API documentation for the core banking system.',
        },
        components: {
            securitySchemes: {
                bearerAuth: {
                    type: 'http',
                    scheme: 'bearer',
                    bearerFormat: 'JWT',
                    description: 'Enter your JWT token in the format: Bearer <token>',
                },
            },
        },
        // Applies the security scheme globally, though it can be overridden per route
        security: [{
            bearerAuth: []
        }],
    },
    // Scans the routes directory for JSDoc annotations
    apis: ['./src/routes/*.js'],
};

const swaggerSpec = swaggerJSDoc(options);

module.exports = swaggerSpec;