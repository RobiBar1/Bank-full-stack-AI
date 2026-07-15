// test/services/health.service.test.js
const HealthService = require('../../src/services/health.service');

describe('HealthService Unit Tests', () => {
    it('should return OK status when the database is connected', async () => {
        const mockRepository = {
            checkDatabaseConnection: jest.fn().mockResolvedValue(true)
        };
        const healthService = new HealthService(mockRepository);

        const metrics = await healthService.getSystemMetrics();

        expect(metrics.status).toBe('OK');
        expect(metrics.database_connected).toBe(true);
        expect(metrics.uptime_seconds).toBeDefined();
    });

    it('should return DEGRADED status when the database is disconnected', async () => {
        const mockRepository = {
            checkDatabaseConnection: jest.fn().mockResolvedValue(false)
        };
        const healthService = new HealthService(mockRepository);

        const metrics = await healthService.getSystemMetrics();

        expect(metrics.status).toBe('DEGRADED');
        expect(metrics.database_connected).toBe(false);
    });
});