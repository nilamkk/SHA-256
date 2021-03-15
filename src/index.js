const express=require('express');
const path=require('path')
const ejs=require('ejs')
const bodyParser = require('body-parser')
const request=require('request')
const calculate=require('../caddon/build/Release/calculate')

const app =express();
app.use(express.json());

const port = process.env.PORT || 7000;

app.use(bodyParser.urlencoded({ extended: true }))
const publicDir = path.join(__dirname, './../public')
app.use(express.static(publicDir))
app.set('view engine', 'ejs')
app.set('views', path.join(__dirname, './../views'));



app.get('/',(req,res)=>
{
    var msg;
    var msgDigest;
    res.render('index',{msg,msgDigest});
})

app.post('/sha-256/output',(req,res)=>
{
    var msg=req.body.inputmsg;
    console.log("++++",req.body,"++++");
    var msgDigest=calculate.calc(msg);
    console.log(msg,msgDigest);
    res.render('index',{msg,msgDigest});
})

app.listen(port, () => console.log("Server is running on port " + port));