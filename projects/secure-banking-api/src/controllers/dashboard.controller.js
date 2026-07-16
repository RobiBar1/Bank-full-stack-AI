// src/controllers/dashboard.controller.js

class DashboardController {
    constructor(dashboardService) {
        this.service = dashboardService;
    }

    /**
     * GET /api/dashboard
     */
    getDashboard = async (req, res, next) => {
        try {
            // req.user is guaranteed to exist and be valid due to the authenticateJWT middleware
            const userId = req.user.id;

            const dashboardData = await this.service.getUserDashboard(userId);

            res.status(200).json({
                message: 'Dashboard data retrieved successfully',
                data: dashboardData
            });
            
        } catch (error) {
            if (error.message === 'User not found.') {
                error.statusCode = 404;
            }
            next(error);
        }
    };
}

module.exports = DashboardController;