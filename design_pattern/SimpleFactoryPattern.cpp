#include<iostream>
#include<string>
//定义一个产品抽象类   要实现的派生类  通过指针继承引用派生类的虚函数方法
//定义一个简单的工厂类 包含产品抽象类
class AbstractSmile
{
private:
    /* data */
public:
    virtual void tranform()=0;
    virtual void ability()=0;
    virtual ~AbstractSmile(){}
};


class SheepSmile:public AbstractSmile
{
private:
    /* data */
public:
     void tranform() override
     {
        std::cout<<"sheep"<<std::endl;
     }
     void ability()override
     {
        std::cout<<"s"<<std::endl;
     }

};


class lionSmile:public AbstractSmile
{
private:
    /* data */
public:
     void tranform() override
     {
        std::cout<<"lion"<<std::endl;
     }
     void ability()override
     {
        std::cout<<"l"<<std::endl;
     }

};



class batSmile:public AbstractSmile
{
private:
    /* data */
public:
     void tranform() override
     {
std::cout<<"bat"<<std::endl;
     }
     void ability()override
     {
        std::cout<<"b"<<std::endl;
     }

};
enum class Type:char{sheep,lion,bat};

class SmileFactory
{
private:
    /* data */
   
public:
 AbstractSmile* createSmile(Type type)
    {
        AbstractSmile* ptr=nullptr;
        switch (type)
        {
        case Type::sheep:
            ptr=new SheepSmile;
            break;

         case Type::lion:
            ptr=new lionSmile;
            break;
        
         case Type::bat:
            ptr=new batSmile;
            break;


        default:
            break;
        }
        return ptr;
    }
};


int main()
{
    SmileFactory* factory=new SmileFactory;
    AbstractSmile* obj=factory->createSmile(Type::lion);
    obj->tranform();
    obj->ability();
    return 0;
}

