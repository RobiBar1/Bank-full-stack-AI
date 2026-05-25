#include <iostream>
#include <string>
#include <vector>

// Component
class FileSystemEntity
{
  public:
    FileSystemEntity(const std::string& name) : name_(name) {}
    virtual ~FileSystemEntity() {}
    virtual FileSystemEntity* Clone() const = 0;

    virtual void Print(int spaces = 0) const = 0;

  protected:
    std::string name_;

    FileSystemEntity(const FileSystemEntity& other) : name_(other.name_) {}
};

// leaf
class File : public FileSystemEntity
{
  public:
    File(const std::string& name) : FileSystemEntity(name) {}

    virtual void Print(int spaces = 0) const
    {
        for (int i = 0; i < spaces; ++i)
        {
            std::cout << "  ";
        }

        std::cout << "- File: " << name_ << std::endl;
    }

    virtual FileSystemEntity* Clone() const { return new File(*this); }

  protected:
    File(const File& other) : FileSystemEntity(other) {}
};

// Composite
class Directory : public FileSystemEntity
{
  public:
    Directory(const std::string& name) : FileSystemEntity(name) {}

    virtual ~Directory()
    {
        for (size_t i = 0; i < children_.size(); ++i)
        {
            delete children_[i];
        }
    }

    void Add(FileSystemEntity* entity) { children_.push_back(entity); }

    virtual void Print(int spaces = 0) const
    {
        for (int i = 0; i < spaces; ++i)
        {
            std::cout << "  ";
        }
        std::cout << "+ Directory: " << name_ << std::endl;

        for (size_t i = 0; i < children_.size(); ++i)
        {
            children_[i]->Print(spaces + 1);
        }
    }

    virtual FileSystemEntity* Clone() const { return new Directory(*this); }

  protected:
    Directory(const Directory& other) : FileSystemEntity(other)
    {
        for (size_t i = 0; i < other.children_.size(); ++i)
        {
            children_.push_back(other.children_[i]->Clone());
        }
    }

  private:
    std::vector<FileSystemEntity*> children_;
};

int main()
{
    Directory* root_dir = new Directory("root");
    Directory* src_dir = new Directory("src");
    Directory* include_dir = new Directory("include");

    File* main_file = new File("main.c");
    File* header_file = new File("utils.h");

    include_dir->Add(header_file);
    src_dir->Add(main_file);
    src_dir->Add(include_dir);
    root_dir->Add(src_dir);

    std::cout << "--- Original Tree ---" << std::endl;
    root_dir->Print();

    FileSystemEntity* cloned_tree = root_dir->Clone();

    std::cout << "\n--- Cloned Tree ---" << std::endl;
    cloned_tree->Print();

    delete root_dir;
    delete cloned_tree;

    return 0;
}