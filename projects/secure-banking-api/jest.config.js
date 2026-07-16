// jest.config.js
module.exports = {
    testEnvironment: 'node',
    // Specifies the script to run before executing the test suites
    setupFilesAfterEnv: ['./test/setup.js'],
    // Automatically clear mock calls and instances between every test
    clearMocks: true,
};