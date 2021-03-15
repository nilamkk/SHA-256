#include<node.h>
#include "stdc++.h"
// #include<bits/stdc++.h>
#define ul unsigned long

using namespace std;

int demo(int x){
    return x+111;
}

//Utility functions signature
vector<ul> string_to_binary(string&);
// vector<ul> pad_to_mod_512bits(vector<ul>);
vector<ul> resize_block(vector<ul>);
vector<vector<ul>> create_chunks(vector<ul>);
string to_hex_(ul);

//Compression algorithm
vector<ul> compress(vector<ul>,vector<ul>&);

//MACROS below for ease of computation
#define ROTR(w,b) ( ( w >> b ) | (w << (32-b)) )
#define BIG_SIG0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22) )
#define BIG_SIG1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25) )
#define SMALL_SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ ((x) >> 3))
#define SMALL_SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))


string main_nohoi(string message)
{
    //Fast IO
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

    // string message;
    // cout<<"Enter your message:"<<endl;
    // cin>>message;

    vector<ul> block;

    block = string_to_binary(message);

    //Create chunks
    vector<vector<ul>> chunks = create_chunks(block);

    vector<ul> H = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };

    for(auto chunk:chunks){
        
        // for(auto ele:chunk ) cout<<"** "<<ele<<endl;
        // cout<<"size --> "<<chunk.size()<<endl;

        vector<ul> resized_block = resize_block(chunk);

        // for(auto ele:resized_block ) cout<<"** "<<ele<<endl;
        // cout<<"size --> "<<resized_block.size()<<endl;

        H = compress(resized_block,H);
    }

    string ans;
    for(auto ele:H) ans+=to_hex_(ele);

    cout<<"*** "<<endl<<ans<<"\n***** "<<endl;
    return ans;

}

vector<vector<ul>> create_chunks(vector<ul> blocks)
{
    ul n = blocks.size();
    int total_size = blocks.size();
    n=n*8;
    ul length_of_input = n;
    n++;
    n+=64;
    int zeros_to_add = 512-(n%512);

    // cout<<"zeros to add --> "<<zeros_to_add<<endl;

    bitset<8> b((1<<7));
    vector<ul> last;
    bitset<64> len(length_of_input);
    // cout<<"len --> "<<len<<endl;
    // cout<<"length_of_input ---> "<<length_of_input<<endl;
    for(int i=0;i<64;i+=8){
        ul t = 0;
        // cout<<"-----------------------------------------------------------------"<<endl;
        for(int j=i;j<i+8;j++){
            t|=(len[63-j]?1:0);
            if(j<i+7) t<<=1ll;
            // cout<<"$$$$$$$$ ---> "<<(len[63-j]?1:0)<<endl;
        }
        // cout<<"####### ---> "<<t<<endl;
        bitset<8> test_val(t);
        // cout<<"^^^^^^^ ---> "<<test_val<<endl;
        last.push_back(t);
    }
    blocks.push_back(b.to_ulong());
    zeros_to_add-=7;
    zeros_to_add/=8;
    while(zeros_to_add--){
        blocks.push_back(0);
    }
    for(auto &ele:last) blocks.push_back(ele);
    int total_chunks = ((int)blocks.size())/64;
    vector<vector<ul>> ans;
    int i = 0;
    while(total_chunks--){
        vector<ul> temp;
        for(int j=i;j<i+64;j++){
            temp.push_back(blocks[j]);
        }
        ans.push_back(temp);
        i+=64;
    }
    return ans;
}

vector<ul> string_to_binary(string &message)
{
    vector<ul> block;
    int n = message.length();
    for(int i=0;i<n;i++){
        bitset<8> b(message[i]);//.c_str() not used ****
        block.push_back(b.to_ulong());
    }
    return block;
}


vector<ul> resize_block(vector<ul> block)
{
    vector<ul> ans(16);
    for(int i=0;i<64;i+=4){
        bitset<32> temp(0);
        temp = block[i] << 24;
		temp |= block[i + 1] << 16;
		temp |= block[i + 2] << 8;
		temp |= block[i + 3];
        ans[i/4] = temp.to_ulong();
    }
    return ans;
}

vector<ul> compress(vector<ul> block, vector<ul> &H)
{

    // for(auto ele:block) cout<<"------->>>> "<<ele<<endl;

    unsigned long k[64] = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};

    ul a = H[0];
    ul b = H[1];
    ul c = H[2];
    ul d = H[3];
    ul e = H[4];
    ul f = H[5];
    ul g = H[6];
    ul h = H[7];

    ul temp1 = 0;
    ul temp2 = 0;

    ul W[64];//Message Schedule

    //to make sure we are mod 32
    for(int t=0;t<=15;t++) W[t] = block[t] & 0xffffffff;

    for(int t=16;t<=63;t++){
        W[t] = SMALL_SIG1(W[t-2]) + W[t-7] + SMALL_SIG0(W[t-15]) + W[t-16];
        W[t] = W[t] & 0xffffffff;
    }

    for(int t=0;t<64;t++){
        temp1 = h + BIG_SIG1(e) + CH(e,f,g) + k[t] + W[t];
        temp2 = BIG_SIG0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = (d+temp1)&0xffffffff;
        d = c;
        c = b;
		b = a;
		a = (temp1 + temp2) & 0xffffffff;
    }

    H[0] = (H[0] + a) & 0xffffffff;
    H[1] = (H[1] + b) & 0xffffffff;
    H[2] = (H[2] + c) & 0xffffffff;
    H[3] = (H[3] + d) & 0xffffffff;

    H[4] = (H[4] + e) & 0xffffffff;
    H[5] = (H[5] + f) & 0xffffffff;
    H[6] = (H[6] + g) & 0xffffffff;
    H[7] = (H[7] + h) & 0xffffffff;

    // for(auto ele:H) cout<<"<<<<<------->>>> "<<ele<<endl;

    return H;

}


string to_hex_(ul input)
{
	bitset<64> b(input & 0xffffffff);
    int a = (unsigned int)b.to_ulong();

    std::stringstream stream;
    stream << std::hex << a;
    std::string result( stream.str() );

    int cnt = 8-result.length();

    while(cnt--) result='0'+result;

    return result;
}


namespace calculate {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::Number;
    using v8::Value;
    using v8::String;

    void Method(const FunctionCallbackInfo<Value>&args ){
        Isolate* isolate=args.GetIsolate();
        // int i=888;
        // i+=demo(i);
        // auto total=Number::New(isolate,i);
        // args.GetReturnValue().Set(total);

        // js String type convert to v8 String type
        v8::String::Utf8Value str(isolate, args[0]);
        std::string cppStr(*str);
        cout<<cppStr<<endl;        
        cppStr=main_nohoi(cppStr);

        // string name= (args[0].As<String>()->Value());
        // // string name= args[0]->ToString();

        // auto total=String::NewFromUtf8(isolate,cppStr);
        // args.GetReturnValue().Set(total);
        // args.GetReturnValue().Set(String::NewFromUtf8(isolate, testString.c_str()).ToLocalChecked());

        args.GetReturnValue().Set(String::NewFromUtf8(isolate, cppStr.c_str()).ToLocalChecked());
        
    }
    void Initialize(Local<Object> exports){
        NODE_SET_METHOD(exports,"calc",Method);
    }
    NODE_MODULE(NDOE_GYP_MODULE_NAME,Initialize);
}




