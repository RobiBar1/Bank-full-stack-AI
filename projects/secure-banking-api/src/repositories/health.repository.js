// src/repositories/health.repository.js

class HealthRepository {
    constructor(dbPool) {
        this.db = dbPool;
    }

    async checkDatabaseConnection() {
        // Executes a lightweight query to verify the PostgreSQL connection is active
        const result = await this.db.query('SELECT 1 AS is_alive');
        return result.rows[0].is_alive === 1;
    }
}

module.exports = HealthRepository;