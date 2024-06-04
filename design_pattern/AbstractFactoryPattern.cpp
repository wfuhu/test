#include<iostream>
 class Shipbody
 {
 private:
    /* data */
 public:
    virtual std::string getbody()=0;
    virtual ~Shipbody()
    {

    }
 };

class WoodBody:public Ship
{
private:
    /* data */
public:
     std::string getbody() override;
     {
        return std::string("木");
     }

};

class IronBody:public Ship
{
private:
    /* data */
public:
     std::string getbody() override;
     {
        return std::string("金属");
     }

};

class metalBody:public Ship
{
private:
    /* data */
public:
     std::string getbody()  override;
     {
        return std::string("合金");
     }

};

class Engine
{
private:
    /* data */
public:
    virtual std::string getEngine()=0;
    virtual ~Engine()
    {

    }
};

class Hum:public Engine
{
private:
    /* data */
public:
     std::string getEngine() override
    {
        return "人工";
    }
   
};

class Diesel:public Engine
{
private:
    /* data */
public:
     std::string getEngine() override
    {
        return "内燃机";
    }
   
};

class nuclear:public Engine
{
private:
    /* data */
public:
     std::string getEngine() override
    {
        return "核反应堆";
    }
   
};

class Weapon
{
private:
    /* data */
public:
   virtual std::string getWeapon()=0;
    virtual ~Weapon()
    {

    }
};

class gun:public Weapon
{
private:
    /* data */
public:
    std::string getWeapon() override
   {
    return "枪";
   }

};

class cannon:public Weapon
{
private:
    /* data */
public:
    std::string getWeapon() override
   {
    return "加农炮";
   }

};

class laser:public Weapon
{
private:
    /* data */
public:
    std::string getWeapon() override
   {
    return "激光";
   }

};

class Ship
{
private:
    Shipbody* m_body;
    Engine* m_engine;
    Weapon* m_weapon;
    /* data */
public:
    Ship(Shipbody* body,Engine* engine, Weapon* weapon):
    m_body{body},m_engine{engine},m_weapon{Weapon}{}
    ~Ship()
    {
    delete m_body;
    delete m_engine;
    delete m_weapon;
    }
    std::string getProperty()
    {
         std::string info=m_body->getbody+m_weapon->getWeapon+m_engine->getEngine;
         return info;
    }
};

class AbstractFactoryPattern
{
private:
    /* data */
public:
    virtual Ship* createShip()=0;
    virtual ~AbstractFactoryPattern();
};

class baseFactory:public AbstractFactory
{
private:
    /* data */
public:
     Ship* createShip() override
    {
        Ship ship=new Ship(new WoodBody,new Hum,new gun);
        return ship;
    }
};


class StandardFactory:public AbstractFactory
{
private:
    /* data */
public:
     Ship* createShip() override
    {
        Ship ship=new Ship(new IronBody,new Diesel,new cannon);
        return ship;
    }
};
class UltimataFactory:public AbstractFactory
{
private:
    /* data */
public:
     Ship* createShip() override
    {
        Ship ship=new Ship(new metalBody,new nuclear,new laser);
        return ship;
    }
};

int main()
{
    AbstractFactory* factory=new UltimataFactory;
    Ship *ship=factory->createShip();
    std::cout<<ship->getProperty();
    delete ship;
    delete factory;
}










 

