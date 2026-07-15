/**
 * Validates the core application state.
 * Returns HTTP 200 along with current process metrics.
 */
const checkHealth = (req, res, next) => {
    try {
        // Exposing process.uptime() is useful for container orchestration (e.g., Kubernetes liveness probes)
        res.status(200).json({
            status: 'OK',
            timestamp: new Date().toISOString(),
            uptime_seconds: process.uptime()
        });
    } catch (error) {
        // Forward to the global error middleware
        next(error);
    }
};

module.exports = {
    checkHealth
};