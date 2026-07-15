// src/services/health.service.js

class HealthService {
    constructor(healthRepository) {
        this.repository = healthRepository;
    }

    async getSystemMetrics() {
        const isDbConnected = await this.repository.checkDatabaseConnection();
        
        return {
            status: isDbConnected ? 'OK' : 'DEGRADED',
            database_connected: isDbConnected,
            timestamp: new Date().toISOString(),
            uptime_seconds: process.uptime()
        };
    }
}

module.exports = HealthService;