const { z, ZodError } = require('zod');
const validate = require('../../src/middlewares/validate.middleware');

describe('Validation Middleware Unit Tests', () => {
    let mockReq, mockRes, mockNext;
    
    // A simple schema for testing purposes
    const testSchema = z.object({
        username: z.string().min(3),
        age: z.number().positive()
    });

    beforeEach(() => {
        mockReq = { body: {} };
        mockRes = {
            status: jest.fn().mockReturnThis(),
            json: jest.fn()
        };
        mockNext = jest.fn();
    });

    it('should call next() and sanitize req.body when validation passes', () => {
        mockReq.body = { username: 'Alice', age: 25, ignoredField: 'drop this' };
        
        const middleware = validate(testSchema);
        middleware(mockReq, mockRes, mockNext);

        expect(mockNext).toHaveBeenCalledTimes(1);
        // Zod strips 'ignoredField' by default unless .passthrough() is used
        expect(mockReq.body).toEqual({ username: 'Alice', age: 25 }); 
    });

    it('should return 400 with formatted errors when validation fails', () => {
        mockReq.body = { username: 'Al', age: -5 };
        
        const middleware = validate(testSchema);
        middleware(mockReq, mockRes, mockNext);

        expect(mockRes.status).toHaveBeenCalledWith(400);
        expect(mockRes.json).toHaveBeenCalledWith(
            expect.objectContaining({
                error: expect.objectContaining({
                    code: 400,
                    message: 'Validation failed',
                    details: expect.any(Array)
                })
            })
        );
        expect(mockNext).not.toHaveBeenCalled();
    });
});