#include <iostream> // istrem, ostrem
#include <queue>
#include <string> // string
#include <unordered_map>

void Active(void)
{
    std::unordered_map<std::string, std::size_t> hashMap;
    std::queue<std::string> orderQue;
    std::string h = "";

    while ("." != h)
    {
        std::cout << "enter your string: " << std::endl;
        std::getline(std::cin, h);

        if (hashMap.end() == hashMap.find(h))
        {
            orderQue.push(h);
            hashMap.insert({h, 1});
        }
        else
        {
            ++(hashMap[h]);
        }
    }

    while (!orderQue.empty())
    {
        h = orderQue.front();
        orderQue.pop();
        if (1 < hashMap[h])
        {
            std::cout << hashMap[h] << " X ";
        }

        std::cout << h << std::endl;
    }
}

int main()
{
    Active();

    return 0;
}