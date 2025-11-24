//实现模板Indexs.但是效率低.而且取时也很麻烦,需要手动传入key,value的数据类型
#include<iostream>
#include<vector>
#include<functional>
#include<string>
#include<cstdint>
#include<limits>

template<typename K>
class Indexes{
public:
std::vector<int>data;
void* key;

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
    
Indexes(K key){
    this->key=new K(key);     
    this->get_indexes(hash(this->key));
    }
void print(){
        std::cout<<"key:"<<this->key<<"\nsize:"<<this->data.size()<<"\ndata:"<<std::endl;
        for(int i=0;i<this->data.size();i++){
            std::cout<<this->data[i]<<" ";}
        std::cout<<std::endl;
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


template<typename T>
class Table{
    public:
    std::vector<TableNode<T>> Nodes;
    std::vector<void*>Values;
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
    template<typename K>
    void save(Indexes<K> indexes,void* value){
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
        if(TempNode==this->NodesMax){//当前未存值
        TempNode->value=Values.size();
        Value v=Value(value,indexes.key);
        Values.push_back(v);
        }
        else{
            
        }
    }
    template<typename K,typename V>
    V* get(Indexes<K> indexes){
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
        return static_cast<V*>(Values[TempNode->value]);
    }
    template<typename k>
    size_t hash(const k& value){//通用哈希函数
        return std::hash<k>{}(value);
    }
    template<typename K>
    Indexes<K> get_indexes(K key){
        Indexes<K> indexes(key);
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

    template<typename K>
    void save(Indexes<K> indexes,void* value){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint16_t>();
            this->typeId=16;
            this->save<K>(indexes,value);
            return;
        }
        table->save<K>(indexes,value);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint32_t>();
            this->typeId=32;
            this->save<K>(indexes,value);
            return;
        }
        table->save<K>(indexes,value);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        if(table->Nodes.size()-1+indexes.data.size()>=table->NodesMAX){
            this->data=table->up<uint64_t>();
            this->typeId=64;
            this->save<K>(indexes,value);
            return;
        }
        table->save<K>(indexes,value);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        table->save<K>(indexes,value);
        break;    
        }
    
    default:
        break;
    }   
    }

    template<typename K,typename V>
    V* get(Indexes<K> indexes){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        return table->get<K,V>(indexes);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
        return table->get<K,V>(indexes);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        return table->get<K,V>(indexes);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        return table->get<K,V>(indexes);
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
    Indexes<int> indexes(1101);
    indexes.print();
    std::string value="hello";
    table.save(indexes,&value);
    std::cout<<"the value is "<<*(table.get<int,std::string>(indexes))<<std::endl;
    table.shrink_to_fit();
    return 0;

}
