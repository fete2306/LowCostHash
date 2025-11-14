#include<iostream>
#include<vector>
#include<functional>
#include<string>
#include<cstdint>
#include<limits>

#define UINT_MAX std::numeric_limits<unsigned int>::max()
#define UINT8_MAX std::numeric_limits<uint8_t>::max()
#define UINT16_MAX std::numeric_limits<uint16_t>::max()
#define UINT32_MAX std::numeric_limits<uint32_t>::max()
#define UINT64_MAX std::numeric_limits<uint64_t>::max()

class Indexs{
public:
std::vector<int>data;
size_t key;

template<typename T>
size_t hash(const T& value){//通用哈希函数
    return std::hash<T>{}(value);
    }

void get_indexs(size_t key){
    while(key>0){
        this->data.push_back(key & 0b11);
        key>>=2;
        }
    }

template<typename T>
Indexs(const T& value){
        get_indexs(hash(value));
        this->key=hash(value);
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
        Table* newTable=new Table<goal>();
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
        delete this;
        return newTable;
    }
    void shrink_to_fit(){
        Nodes.shrink_to_fit();
        Values.shrink_to_fit();
    }
    void save(Indexs indexs,void* value){
        TableNode<T>* TempNode=&Nodes[0];
        for(int i=0;i<indexs.data.size();i++){
            int index=indexs.data[i];
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
                    std::cout<<"[save]索引越界"<<" indexs["<<i<<"]="<<index<<std::endl;
                    break;
            }
            if(*index_Node==this->NodesMAX){
                *index_Node=Nodes.size();
                Nodes.push_back(TableNode());
            }
            TempNode=&Nodes[*index_Node];
        }
        TempNode->value=Values.size();
        Values.push_back(value);
    }
    template<typename v>
    v* get(Indexs indexs){
        TableNode<T>* TempNode=&Nodes[0];
        for(int i=0;i<indexs.data.size();i++){
            int index=indexs.data[i];
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
                    std::cout<<"[get]索引越界"<<" indexs["<<i<<"]="<<index<<std::endl;
                    break;
            }
            if(*index_Node==this->NodesMAX){
                *index_Node=Nodes.size();
                Nodes.push_back(TableNode());
            }
            TempNode=&Nodes[*index_Node];
        }
        return static_cast<v*>(Values[TempNode->value]);
    }
    template<typename k>
    size_t hash(const k& value){//通用哈希函数
        return std::hash<k>{}(value);
    }

    Indexs get_indexs(size_t key){
        Indexs indexs(key);
        return indexs;
    }

};

class LowCostHash{
    public:
    void* data;
    uint8_t typeId=8;

    LowCostHash(){
        data=new Table<uint8_t>();
    }

    void save(Indexs indexs,void* value){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        if(table->Nodes.size()-1+indexs.data.size()>=table->NodesMAX){
            this->data=table->up<uint16_t>();
            this->typeId=16;
            this->save(indexs,value);
        }
        table->save(indexs,value);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);
        if(table->Nodes.size()-1+indexs.data.size()>=table->NodesMAX){
            this->data=table->up<uint32_t>();
            this->typeId=32;
            this->save(indexs,value);
        }
        table->save(indexs,value);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        if(table->Nodes.size()-1+indexs.data.size()>=table->NodesMAX){
            this->data=table->up<uint64_t>();
            this->typeId=64;
            this->save(indexs,value);
        }
        table->save(indexs,value);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        table->save(indexs,value);
        break;    
        }
    
    default:
        break;
    }   
    }
    template<typename v>
    v* get(Indexs indexs){
    switch (this->typeId)
    {
    case 8:{
        Table<uint8_t>* table=static_cast<Table<uint8_t>*>(this->data);
        return table->get<v>(indexs);
        break;
    }
    case 16:{
        Table<uint16_t>* table=static_cast<Table<uint16_t>*>(this->data);

        return table->get<v>(indexs);
        break;
    }
        
    case 32:{
        Table<uint32_t>* table=static_cast<Table<uint32_t>*>(this->data);
        
        return table->get<v>(indexs);
        break;
    }
    case 64:{
        Table<uint64_t>* table=static_cast<Table<uint64_t>*>(this->data);
        return table->get<v>(indexs);
        break;    
        }
    
    default:
        break;
    }   
    }

}


int main(){
    LowCostHash table=LowCostHash();
    Indexs indexs=Indexs(1101);
    indexs.print();
    std::string value="hello";
    table.save(indexs,&value);
    std::cout<<"the value is "<<*(table.get_value<std::string>(indexs))<<std::endl;
    table.shrink_to_fit();
    return 0;

}
