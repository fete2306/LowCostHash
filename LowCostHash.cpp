#include<iostream>
#include<vector>
#include<functional>
#include<string>
#include<cstdint>
#include<limits>

class DATA{
public:
    void* data;
    const std::type_info* id;
    std::function<bool(DATA*,DATA* )> _equal;
    
    DATA() : data(nullptr), id(nullptr) {} //默认，占位用

    template<typename T>
    DATA(T data){
        this->data=new T(data);
        this->id=&typeid(T);
        this->_equal= [this](DATA* self,DATA* other){ return this->equal_template<T>(self,other); };//避免拷贝时函数体状态错乱,所以不使用this参数
    }

    bool equal(DATA* other){
        return this->_equal(this,other);
    }

    template<typename T>
    bool equal_template(DATA* self,DATA* other){
        if(*(self->id)!=*(other->id))return false;
        T* a=static_cast<T*>(self->data);
        T* b=static_cast<T*>(other->data);
        return *a==*b;
    }
};

class Indexes{
public:
std::vector<int>data;
DATA key;

template<typename k>
size_t hash(const k& key){//通用哈希函数
    return std::hash<k>{}(key);
    }

void get_indexes(size_t key){
    while(key>0){
        this->data.push_back(key & 0b11);
        key>>=2;
        }
    }

template<typename K>
Indexes(K key){
    this->key=DATA(key);    
    this->get_indexes(hash(key));
    }

void print(){
    std::cout<<"the data ="<<std::endl<<"[";
    for(int i=0;i<this->data.size();i++){
        std::cout<<this->data[i]<<" ";
    }
    std::cout<<"]"<<std::endl;
    return;
}

};

template<typename T>
class TableNode{
    public:
    T p0=std::numeric_limits<T>::max();
    T p1=std::numeric_limits<T>::max();
    T p2=std::numeric_limits<T>::max();
    T p3=std::numeric_limits<T>::max();
    T value=std::numeric_limits<T>::max();
};

class Value{
    public:
    void* data;
    DATA key;
    unsigned int next=std::numeric_limits<unsigned int>::max();

    template<typename V>
    Value(V* data,DATA key){
        this->data=new V(*data);
        this->key=key;
    }

    bool key_equal(DATA* other){
    return this->key.equal(other);        
    }

};

template<typename T>
class Table{
    public:
    std::vector<TableNode<T>> Nodes;
    std::vector<Value>Values;
    T NodesMAX=std::numeric_limits<T>::max();

    Table() {
        Nodes.push_back(TableNode<T>()); 
    }
    template<typename goal>
    Table<goal>* up(){
        Table<goal>* newTable=new Table<goal>();
        newTable->Nodes.reserve(this->Nodes.size());
        for(int i=0;i<this->Nodes.size();i++){
            TableNode<goal>Temp=TableNode<goal>();
            Temp.p0=static_cast<goal>(this->Nodes[i].p0);
            Temp.p1=static_cast<goal>(this->Nodes[i].p1);
            Temp.p2=static_cast<goal>(this->Nodes[i].p2);
            Temp.p3=static_cast<goal>(this->Nodes[i].p3);
            Temp.value=static_cast<goal>(this->Nodes[i].value);
            newTable->Nodes.push_back(Temp);
        }
        newTable->Values=this->Values;
        delete this;
        return newTable;
    }
    
    void shrink_to_fit(){
        Nodes.shrink_to_fit();
        Values.shrink_to_fit();
    }
    template<typename V>
    void save(Indexes indexes,V* value){
        TableNode<T>* TempNode=&Nodes[0];
        for(int i=0;i<indexes.data.size();i++){
            int index=indexes.data[i];
            T* index_Node=nullptr;
            switch(index){
                case 0:
                    index_Node=&TempNode->p0;
                    break;
                case 1:
                    index_Node=&TempNode->p1;
                    break;
                case 2: 
                    index_Node=&TempNode->p2;
                    break;
                case 3:
                    index_Node=&TempNode->p3;
                    break;
                default:
                    std::cout<<"[save]索引越界"<<" indexes["<<i<<"]="<<index<<std::endl;
                    break;
            }
            if(*index_Node==this->NodesMAX){
                *index_Node=Nodes.size();
                Nodes.push_back(TableNode<T>());
            }
            TempNode=&Nodes[*index_Node];
        }
        if(TempNode->value==this->NodesMAX){//当前未存值
        TempNode->value=Values.size();
        Value v=Value(value,indexes.key);//接受模板参数用来在堆上创建对象
        Values.push_back(v);
        }
        else{
        if(Values[TempNode->value].key_equal(&indexes.key)){//键相同
            Values[TempNode->value].data=value;
        }
        else{//哈希冲突
            Values[TempNode->value].next=Values.size();
            Value v=Value(value,indexes.key);
            Values.push_back(v);
        }
        }
    }
    template<typename V>
    V* get(Indexes indexes){
        TableNode<T>* TempNode=&Nodes[0];
        for(int i=0;i<indexes.data.size();i++){
            int index=indexes.data[i];
            T* index_Node=nullptr;
            switch(index){
                case 0:
                    index_Node=&TempNode->p0;
                    break;
                case 1:
                    index_Node=&TempNode->p1;
                    break;
                case 2: 
                    index_Node=&TempNode->p2;
                    break;
                case 3:
                    index_Node=&TempNode->p3;
                    break;
                default:
                    std::cout<<"[get]索引越界"<<" indexes["<<i<<"]="<<index<<std::endl;
                    break;
            }
            TempNode=&Nodes[*index_Node];
        }
        if(TempNode->value!=this->NodesMAX){//值存在
            if(Values[TempNode->value].key_equal(&indexes.key)){//同一个键
                return static_cast<V*>(Values[TempNode->value].data);
            }
            else{
            unsigned int temp=Values[TempNode->value].next;
            while(!Values[temp].key_equal(&indexes.key)){//循环查找
                if(temp==std::numeric_limits<unsigned int>::max()){
                    std::cout<<"[get] 不存在包含对象键的桶"<<std::endl;
                    return nullptr;
                }
                temp=Values[temp].next;
            }
            return static_cast<V*>(Values[temp].data);
            }
        }
        std::cout<<"[get] 对应的键未存储值"<<std::endl;
        return nullptr;
    }

    template<typename k>
    size_t hash(const k& value){//通用哈希函数
        return std::hash<k>{}(value);
    }

    template<typename K>
    Indexes get_indexes(K key){
        Indexes indexes = Indexes(key);
        return indexes;
    }
    

};

class LowCostHash{
    public:
    void* data;
    uint8_t typeId=8;

    LowCostHash(){
        data=new Table<uint8_t>();
    }
    void shrink_to_fit(){
        switch (this->typeId)
        {
        case 8:{
            Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
            table->shrink_to_fit();
            break;
        }
        case 16:{
            Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
            table->shrink_to_fit();
            break;
        }
        case 32:{
            Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
            table->shrink_to_fit();
            break;
        }
        case 64:{
            Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
            table->shrink_to_fit();
            break;    
            }
        default:
        break;
        }
        }

    template<typename V>
    void save(Indexes indexes,V* value){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint16_t>();
            this->typeId=16;
            this->save<V>(indexes,value);
            return;
        }
        table->save<V>(indexes,value);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint32_t>();
            this->typeId=32;
            this->save<V>(indexes,value);
            return;
        }
        table->save<V>(indexes,value);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint64_t>();
            this->typeId=64;
            this->save<V>(indexes,value);
            return;
        }
        table->save<V>(indexes,value);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        table->save<V>(indexes,value);
        break;    
        }
    
    default:
        break;
    }   
    }

    template<typename V>
    V* get(Indexes indexes){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        return table->get<V>(indexes);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
        return table->get<V>(indexes);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        return table->get<V>(indexes);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        return table->get<V>(indexes);
        break;    
        }
    
    default:
        std::cout<<"get failed"<<std::endl;
        return nullptr;
        break;
    }   
    }

};

int main(){
    LowCostHash table=LowCostHash();
    Indexes indexes = Indexes(1101);
    std::string value="love";
    table.save(indexes,&value);
    indexes.print();
    std::cout<<"the value is "<<*(table.get<std::string>(indexes))<<std::endl;

    Indexes indexes1=Indexes(1101);
    indexes1.key=1314;
    std::string value1="hope";
    table.save(indexes1,&value1);
    indexes1.print();
    std::cout<<"the value is "<<*(table.get<std::string>(indexes1))<<std::endl;

    table.shrink_to_fit();
    return 0;

}
