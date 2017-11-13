
//--------------------------------------------------
// ˆ³k
//--------------------------------------------------
unsigned int Compress(unsigned char *d,size_t d_len,unsigned char *s,size_t s_len){
	//¦ŠÈˆÕ”Å
	if(!d)return s_len+s_len/0x3F+4;
	for(;s_len>=0x3F;s_len-=0x3F){
		*(d++)=0x3F;
		for(int i=0x3F;i>0;i--)*(d++)=*(s++);
	}
	*(d++)=(unsigned char)s_len;
	for(;s_len>0;s_len--)*(d++)=*(s++);
	*(d++)=0;
	*(d++)=0;
	*(d++)=0;
	return d_len;
}

//--------------------------------------------------
// ‰ð“€
//--------------------------------------------------
unsigned int Decompress(unsigned char *d,size_t d_len,unsigned char *s,size_t s_len){
	d_len=0;
	for(unsigned int i=0;i<s_len;){
		int len,disp,c=s[i++];
		if(c<0x40){
			if(c){
				len=c;
			}else if(s[i]||s[i+1]){
				int f=s[i++];
				if(f&0x80)len=(f&0x7F)+0x40;
				else len=(f<<8)+s[i++]+0xBF;
			}else{
				break;
			}
			if(d)for(;len>0;len--)d[d_len++]=s[i++];
			else{d_len+=len;i+=len;}
			len=0;
		}else if(c<0x80){
			len=((c&0x30)>>4)+2;
			disp=(c&0x0F)+1;
		}else if(c<0xC0){
			len=((c&0x3C)>>2)+3;
			disp=((c&0x03)<<8)+s[i++]+1;
		}else{
			len=((c&0x3F)<<1)+((s[i]&0x80)>>7)+4;
			disp=((s[i]&0x7F)<<8)+s[i+1]+1;i+=2;
		}
		if(d)for(int j=0;j<len;j++)d[d_len++]=d[d_len-disp];
		else d_len+=len;
	}
	return d_len;
}
