#include<iostream>
#include<string>
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

class AbstractFactory
{
private:
    /* data */
   
public:
     virtual AbstractFactory* creatSmile()=0;
     virtual ~AbstractFactory(){}
};

class sheepFactory:public AbstractFactory
{
private:
    /* data */
public:
      AbstractFactory* creatSmile()
      {
        return new sheepFactory;
      }
      ~AbstractFactory(){}
};


class lionFactory:public AbstractFactory
{
private:
    /* data */
public:
      AbstractFactory* creatSmile()
      {
        return new lionSmile;
      }
      ~AbstractFactory(){}
};

class batFactory:public AbstractFactory
{
private:
    /* data */
public:
      AbstractFactory* creatSmile()
      {
        return new batSmile;
      }
      ~AbstractFactory(){}
};




int main()
{
    SmileFactory* factory=new SmileFactory;
    AbstractSmile* obj=factory->createSmile(Type::lion);
    obj->tranform();
    obj->ability();
    delete obj;
    delete factory;
    return 0;
}

