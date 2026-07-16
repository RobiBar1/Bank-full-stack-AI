const UserRepository = require('../../src/repositories/user.repository');

describe('UserRepository Unit Tests', () => {
    let mockDb;
    let userRepository;

    beforeEach(() => {
        mockDb = {
            query: jest.fn()
        };
        userRepository = new UserRepository(mockDb);
    });

    it('should find a user by email using the default pool', async () => {
        const mockUser = { id: '123', email: 'test@test.com' };
        mockDb.query.mockResolvedValue({ rows: [mockUser] });

        const result = await userRepository.findByEmail('test@test.com');

        expect(mockDb.query).toHaveBeenCalledWith(
            'SELECT * FROM users WHERE email = $1',
            ['test@test.com']
        );
        expect(result).toEqual(mockUser);
    });

    it('should use an injected transaction client if provided', async () => {
        const mockClient = { query: jest.fn().mockResolvedValue({ rows: [] }) };
        
        await userRepository.findByEmail('test@test.com', mockClient);

        expect(mockClient.query).toHaveBeenCalled();
        expect(mockDb.query).not.toHaveBeenCalled();
    });
});