#include <iostream>
#include <string>
#include <vector>


// class Neuron;

template<typename Derived>
class SomeNeurons {
public:
    template<typename T>
    void connect_to(T& other)
    {
        for (auto& from : self())
        {
            for (auto& to : other)
            {
                from.push_out(&to);
                to.push_in(&from);
            }
        }
    }

private:
    friend Derived;
    Derived& self() & noexcept { return *static_cast<Derived*>(this); }
    Derived const& self() const& noexcept { return *static_cast<Derived const*>(this); }
    Derived&& self() && noexcept { return *static_cast<Derived*>(this); }
};


class Neuron : public SomeNeurons<Neuron>
{
public:
    friend SomeNeurons<Neuron>;
    Neuron()
        : id{++Neuron::neuron_id}
    {
    }

    Neuron* begin() noexcept { return this; }
    Neuron* end() noexcept { return this+1; }
    Neuron const* begin() const noexcept { return this; }
    Neuron const* end() const noexcept { return this+1; }

    template<typename T>
    void push_out(T* pneuron)
    {
        static_assert(std::is_convertible_v<T, Neuron>);
        out.push_back(pneuron);
    }

    template<typename T>
    void push_in(T* pneuron)
    {
        static_assert(std::is_convertible_v<T, Neuron>);
        in.push_back(pneuron);
    }

    friend std::ostream& operator<<(std::ostream& os, Neuron const& obj)
    {
        for (Neuron const* n : obj.in)
        {
            os << n->id << "\t-->\t[" << obj.id << "]\n";
        }
        for (Neuron const* n : obj.out)
        {
            os << "[" << obj.id << "]\t-->\t" << n->id << "\n";
        }
        return os;
    }

private:
    std::vector<Neuron*> in{}, out{};
    int id;
    static inline int neuron_id{};
};

class NeuronLayer : private std::vector<Neuron>, public SomeNeurons<NeuronLayer> {
    using Storage = std::vector<Neuron>;
public:
    explicit NeuronLayer(int count)
        : Storage(count)
    {
    }

    decltype(auto) begin() { return Storage::begin(); }
    decltype(auto) end() { return Storage::end(); }
    decltype(auto) begin() const { return Storage::begin(); }
    decltype(auto) end() const { return Storage::end(); }

    friend std::ostream& operator<<(std::ostream& os, NeuronLayer const& obj)
    {
        for (auto& n : obj) { os << n; }
        return os;
    }
};


int main()
{
    Neuron n1, n2;
    n1.connect_to(n2);

    std::cerr << n1 << n2 << "\n";

    NeuronLayer l1{5};
    Neuron n3;
    l1.connect_to(n3);
    std::cerr << "Neuron:\n" << n3 << "\n";
    std::cerr << "Layer:\n" << l1 << "\n";

    NeuronLayer l2{2}, l3{3};
    l2.connect_to(l3);
    std::cerr << "Layer l2:\n" << l2 << "\n";
    std::cerr << "Layer l3:\n" << l3 << "\n";
}
