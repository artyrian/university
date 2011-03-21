class B{
    public: 
    int val1;
    B(int val) : val1(val) {};
    virtual ~B() {}
    virtual void g() {}
    
};

class A : public B {
    public:
    int val2;
    void g() {}
    A(int val1_, int val2_);
};
