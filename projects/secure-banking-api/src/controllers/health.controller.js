// src/controllers/health.controller.js

class HealthController {
    constructor(healthService) {
        this.service = healthService;
    }

    checkHealth = async (req, res, next) => {
        try {
            const metrics = await this.service.getSystemMetrics();
            const statusCode = metrics.status === 'OK' ? 200 : 503;
            
            res.status(statusCode).json(metrics);
        } catch (error) {
            next(error);
        }
    };
}

module.exports = HealthController;