if(typeof(SAPO)=='undefined'){window.SAPO={};}else{window.SAPO=window.SAPO;}
SAPO.namespace=function(ns){if(!ns||!ns.length){return null;}
var levels=ns.split(".");var nsobj=SAPO;for(var i=(levels[0]=="SAPO")?1:0;i<levels.length;++i){nsobj[levels[i]]=nsobj[levels[i]]||{};nsobj=nsobj[levels[i]];}
return nsobj;};function s$(element)
{if(arguments.length>1){for(var i=0,elements=[],length=arguments.length;i<length;i++){elements.push(s$(arguments[i]));}
return elements;}
if(typeof(element)=='string'){element=document.getElementById(element);}
return element;}
Function.prototype.bindObj=function(){if(arguments.length<2&&arguments[0]===undefined){return this;}
var __method=this;var args=[];for(var i=0,total=arguments.length;i<total;i++){args.push(arguments[i]);}
var object=args.shift();return function(){return __method.apply(object,args.concat(function(tmpArgs){var args2=[];for(var j=0,total=tmpArgs.length;j<total;j++){args2.push(tmpArgs[j]);}
return args2;}(arguments)));}};Function.prototype.bindObjEvent=function(){var __method=this;var args=[];for(var i=0;i<arguments.length;i++){args.push(arguments[i]);}
var object=args.shift();return function(event){return __method.apply(object,[event||window.event].concat(args));}};Object.extend=function(destination,source){for(var property in source){destination[property]=source[property];}
return destination;};if(typeof(SAPO.Browser)=='undefined'){SAPO.Browser={IE:false,GECKO:false,OPERA:false,SAFARI:false,KONQUEROR:false,model:false,version:false,userAgent:false,init:function()
{this.detectBrowser();this.setDimensions();this.setReferrer();},setDimensions:function()
{var myWidth=0,myHeight=0;if(typeof(window.innerWidth)=='number'){myWidth=window.innerWidth;myHeight=window.innerHeight;}else if(document.documentElement&&(document.documentElement.clientWidth||document.documentElement.clientHeight)){myWidth=document.documentElement.clientWidth;myHeight=document.documentElement.clientHeight;}else if(document.body&&(document.body.clientWidth||document.body.clientHeight)){myWidth=document.body.clientWidth;myHeight=document.body.clientHeight;}
this.windowWidth=myWidth;this.windowHeight=myHeight;},setReferrer:function()
{this.referrer=document.referrer!==undefined?document.referrer.length>0?escape(document.referrer):false:false;},detectBrowser:function()
{var sAgent=navigator.userAgent;this.userAgent=sAgent;sAgent=sAgent.toLowerCase();if((new RegExp("applewebkit\/")).test(sAgent)){this.SAFARI=true;this.model='safari';this.version=sAgent.replace(new RegExp("(.*)applewebkit\/([^\\s]+)(.*)"),"$2");}else if((new RegExp("opera")).test(sAgent)){this.OPERA=true;this.model='opera';this.version=sAgent.replace(new RegExp("(.*)opera.([^\\s$]+)(.*)"),"$2");}else if((new RegExp("konqueror")).test(sAgent)){this.KONQUEROR=true;this.model='konqueror';this.version=sAgent.replace(new RegExp("(.*)konqueror\/([^;]+);(.*)"),"$2");}else if((new RegExp("msie\ ")).test(sAgent)){this.IE=true;this.model='ie';this.version=sAgent.replace(new RegExp("(.*)\\smsie\\s([^;]+);(.*)"),"$2");}else if((new RegExp("gecko")).test(sAgent)){this.GECKO=true;var re=new RegExp("(camino|chimera|epiphany|minefield|firefox|firebird|phoenix|galeon|iceweasel|k\-meleon|seamonkey|netscape|songbird|sylera)");if(re.test(sAgent)){this.model=sAgent.match(re)[1];this.version=sAgent.replace(new RegExp("(.*)"+this.model+"\/([^;\\s$]+)(.*)"),"$2");}else{this.model='mozilla';var reVersion=new RegExp("(.*)rv\:([^\)]+)(.*)");if(reVersion.test(sAgent)){this.version=sAgent.replace(reVersion,"$2");}}}},debug:function()
{var str="known browsers: (ie, gecko, opera, safari, konqueror) \n";str+=[this.IE,this.GECKO,this.OPERA,this.SAFARI,this.KONQUEROR]+"\n";str+="model -> "+this.model+"\n";str+="version -> "+this.version+"\n";str+="\n";str+="original UA -> "+this.userAgent;alert(str);}};SAPO.Browser.init();}
SAPO.logReferer=function(classURL){var thisOptions=Object.extend({s:'js.sapo.pt',swakt:'59a97a5f-0924-3720-a62e-0c44d9ea4f16',pg:false,swasection:false,ref:false,etype:'libsapojs-view',swav:'1',swauv:'1',bcs:'1',bsr:'1',bul:'1',bje:'1',bfl:'1',debug:false},arguments[1]||{});if(typeof(classURL)!='undefined'&&classURL!=null){if(!thisOptions.pg){thisOptions.pg=classURL;}
if(!thisOptions.swasection){thisOptions.swasection=classURL;}
if(!thisOptions.ref){thisOptions.ref=location.href;}
var waURI='http://wa.sl.pt/wa.gif?';var waURISSL='https://ssl.sapo.pt/wa.sl.pt/wa.gif?';var aQuery=['pg='+encodeURIComponent(thisOptions.pg),'swasection='+encodeURIComponent(thisOptions.swasection),'s='+thisOptions.s,'ref='+encodeURIComponent(thisOptions.ref),'swakt='+thisOptions.swakt,'etype='+encodeURIComponent(thisOptions.etype),'swav='+encodeURIComponent(thisOptions.swav),'swauv='+encodeURIComponent(thisOptions.swauv),'bcs='+encodeURIComponent(thisOptions.bcs),'bsr='+encodeURIComponent(thisOptions.bsr),'bul='+encodeURIComponent(thisOptions.bul),'bje='+encodeURIComponent(thisOptions.bje),'bfl='+encodeURIComponent(thisOptions.bfl),''];if(location.protocol=='https:'){var waLogURI=waURISSL;}else{var waLogURI=waURI;}
var img=new Image();img.src=waLogURI+aQuery.join('&');}};SAPO._require=function(uri,callBack)
{if(typeof(uri)!='string'){return;}
var script=document.createElement('script');var aHead=document.getElementsByTagName('HEAD');if(aHead.length>0){aHead[0].appendChild(script);}
script.type='text/javascript';if(!document.attachEvent){script.onload=function(e){if(typeof(callBack)!='undefined'){callBack();}};}else{script.onreadystatechange=function(e){if(this.readyState=='loaded'){if(typeof(callBack)!='undefined'){callBack();}}};}
script.src=uri;};SAPO.require=function(reqArray,callBack)
{var objectsToCheck=[];var uriToAdd=[];var _isSAPOObject=function(param){if(typeof(param)=='string'){if(/^SAPO\./.test(param)){return true;}}
return false;};var _isObjectUri=function(param){if(typeof(param)=='object'&&param.constructor==Object){if(typeof(param.uri)=='string'){return true;}}
return false;};var _isObjectArray=function(param){if(typeof(param)=='object'&&param.constructor==Array){return true;}
return false;};var _parseSAPOObject=function(param){var aSAPO=param.split('.');var sapoURI=aSAPO.join('/');sapoURI='http://js.sapo.pt/'+sapoURI+'/';return sapoURI;};var _parseObjectUri=function(param)
{return param.uri;}
var _objectExists=function(objStr){if(typeof(objStr)!='undefined'){var aStrObj=objStr.split('.');var objParent=window;for(var k=0,aStrObjLength=aStrObj.length;k<aStrObjLength;k++){if(typeof(objParent[aStrObj[k]])!='undefined'){objParent=objParent[aStrObj[k]];}else{return false;}}
return true;}};var requestRecursive=function()
{if(uriToAdd.length>1){SAPO._require(uriToAdd[0],requestRecursive);uriToAdd.splice(0,1);}else if(uriToAdd.length==1){if(typeof(callBack)!='undefined'){SAPO._require(uriToAdd[0],callBack);}else{SAPO._require(uriToAdd[0]);}
uriToAdd.splice(0,1);}else if(uriToAdd.length===0){if(typeof(callBack)!='undefined'){callBack();}}};if(typeof(reqArray)!='undefined'){var cur=false;var curURI=false;if(typeof(reqArray)=='string'){if(_isSAPOObject(reqArray)){if(!_objectExists(reqArray)){uriToAdd.push(_parseSAPOObject(reqArray));}}else{uriToAdd.push(reqArray);}}else{for(var i=0,reqArrayLength=reqArray.length;i<reqArrayLength;i++){cur=reqArray[i];if(_isSAPOObject(cur)){if(!_objectExists(cur)){objectsToCheck.push(cur);uriToAdd.push(_parseSAPOObject(cur));}}else if(_isObjectArray(cur)){if(cur.length>0){if(_isSAPOObject(cur[0])){if(!_objectExists(cur[0])){if(cur.length===2){uriToAdd.push(_parseSAPOObject(cur[0])+cur[1]+'/');}else{uriToAdd.push(_parseSAPOObject(cur[0]));}}}}}else{if(typeof(cur)=='string'){uriToAdd.push(cur);}else{if(_isObjectUri(cur)){if(typeof(cur.check)=='string'){if(!_objectExists(cur.check)){uriToAdd.push(_parseObjectUri(cur));}}else{uriToAdd.push(_parseObjectUri(cur));}}}}}}
if(arguments.length==3){if(typeof(arguments[2])=='boolean'){if(arguments[2]===true){for(var l=0,uriToAddLength=uriToAdd.length;l<uriToAddLength;l++){SAPO._require(uriToAdd[l]);}
if(typeof(callBack)!='undefined'){callBack();}
return;}}
requestRecursive();}else{requestRecursive();}}};
if(!SAPO.Utility||typeof(SAPO.Utility)=='undefined'){SAPO.namespace('Utility');}
SAPO.Utility.Dumper={_tab:'\xA0\xA0\xA0\xA0',_formatParam:function(param)
{var formated='';switch(typeof(param)){case'string':formated='(string) '+param;break;case'number':formated='(number) '+param;break;case'boolean':formated='(boolean) '+param;break;case'object':if(param!=null){if(param.constructor==Array){formated='Array \n{\n'+this._outputFormat(param,0)+'\n}';}else if(param.constructor==Object){formated='Object \n{\n'+this._outputFormat(param,0)+'\n}';}}else{formated='null';}
break;default:formated=false;}
return formated;},_getTabs:function(numberOfTabs)
{var tabs='';for(var _i=0;_i<numberOfTabs;_i++){tabs+=this._tab;}
return tabs;},_outputFormat:function(param,dim)
{var formated='';var _strVal=false;var _typeof=false;for(var key in param){if(param[key]!=null){if(typeof(param[key])=='object'&&(param[key].constructor==Array||param[key].constructor==Object)){if(param[key].constructor==Array){_typeof='Array';}else if(param[key].constructor==Object){_typeof='Object';}
formated+=this._tab+this._getTabs(dim)+'['+key+'] => <b>'+_typeof+'</b>\n';formated+=this._tab+this._getTabs(dim)+'{\n';formated+=this._outputFormat(param[key],dim+1)+this._tab+this._getTabs(dim)+'}\n';}else if(param[key].constructor==Function){continue;}else{formated=formated+this._tab+this._getTabs(dim)+'['+key+'] => '+param[key]+'\n';}}else{formated=formated+this._tab+this._getTabs(dim)+'['+key+'] => null \n';}}
return formated;},printDump:function(param,target)
{if(!target||typeof(target)=='undefined'){document.write('<pre>'+this._formatParam(param)+'</pre>');}else{if(typeof(target)=='string'){document.getElementById(target).innerHTML='<pre>'+this._formatParam(param)+'</pre>';}else if(typeof(target)=='object'){target.innerHTML='<pre>'+this._formatParam(param)+'</pre>';}else{throw"TARGET must be an element or an element ID";}}},returnDump:function(param)
{return this._formatParam(param);},alertDump:function(param)
{alert(this._formatParam(param).replace(/(<b>)(Array|Object)(<\/b>)/g,"$2"));},windowDump:function(param)
{var dumperwindow='dumperwindow_'+(Math.random()*10000);var win=window.open('',dumperwindow,'width=400,height=300,left=50,top=50,status,menubar,scrollbars,resizable');win.document.open();win.document.write('<pre>'+this._formatParam(param)+'</pre>');win.document.close();win.focus();}};
