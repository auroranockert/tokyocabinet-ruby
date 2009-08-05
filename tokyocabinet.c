/*************************************************************************************************
 * Ruby binding of Tokyo Cabinet
 *                                                      Copyright (C) 2006-2007 Mikio Hirabayashi
 * This file is part of Tokyo Cabinet.
 * Tokyo Cabinet is free software; you can redistribute it and/or modify it under the terms of
 * the GNU Lesser General Public License as published by the Free Software Foundation; either
 * version 2.1 of the License or any later version.  Tokyo Cabinet is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with Tokyo
 * Cabinet; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 *************************************************************************************************/


#include "ruby.h"
#include <tcutil.h>
#include <tchdb.h>
#include <tcbdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define HDBVNDATA      "hdb"
#define BDBVNDATA      "bdb"
#define BDBCURVNDATA   "bdbcur"


/* private function prototypes */
static void hdb_init(void);
static VALUE hdb_initialize(VALUE vself);
static VALUE hdb_errmsg(VALUE vself, VALUE vecode);
static VALUE hdb_ecode(VALUE vself);
static VALUE hdb_tune(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_setcache(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_open(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_close(VALUE vself);
static VALUE hdb_put(VALUE vself, VALUE vkey, VALUE vval);
static VALUE hdb_putkeep(VALUE vself, VALUE vkey, VALUE vval);
static VALUE hdb_putcat(VALUE vself, VALUE vkey, VALUE vval);
static VALUE hdb_putasync(VALUE vself, VALUE vkey, VALUE vval);
static VALUE hdb_out(VALUE vself, VALUE vkey);
static VALUE hdb_get(VALUE vself, VALUE vkey);
static VALUE hdb_vsiz(VALUE vself, VALUE vkey);
static VALUE hdb_iterinit(VALUE vself);
static VALUE hdb_iternext(VALUE vself);
static VALUE hdb_fwmkeys(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_sync(VALUE vself);
static VALUE hdb_optimize(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_vanish(VALUE vself);
static VALUE hdb_copy(VALUE vself, VALUE vpath);
static VALUE hdb_path(VALUE vself);
static VALUE hdb_rnum(VALUE vself);
static VALUE hdb_fsiz(VALUE vself);
static VALUE hdb_fetch(int argc, VALUE *argv, VALUE vself);
static VALUE hdb_check(VALUE vself, VALUE vkey);
static VALUE hdb_check_value(VALUE vself, VALUE vval);
static VALUE hdb_get_reverse(VALUE vself, VALUE vval);
static VALUE hdb_empty(VALUE vself);
static VALUE hdb_each(VALUE vself);
static VALUE hdb_each_key(VALUE vself);
static VALUE hdb_each_value(VALUE vself);
static VALUE hdb_keys(VALUE vself);
static VALUE hdb_values(VALUE vself);
static void bdb_init(void);
static VALUE bdb_initialize(VALUE vself);
static VALUE bdb_errmsg(VALUE vself, VALUE vecode);
static VALUE bdb_ecode(VALUE vself);
static VALUE bdb_tune(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_setcache(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_open(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_close(VALUE vself);
static VALUE bdb_put(VALUE vself, VALUE vkey, VALUE vval);
static VALUE bdb_putkeep(VALUE vself, VALUE vkey, VALUE vval);
static VALUE bdb_putcat(VALUE vself, VALUE vkey, VALUE vval);
static VALUE bdb_putdup(VALUE vself, VALUE vkey, VALUE vval);
static VALUE bdb_putlist(VALUE vself, VALUE vkey, VALUE vvals);
static VALUE bdb_out(VALUE vself, VALUE vkey);
static VALUE bdb_outlist(VALUE vself, VALUE vkey);
static VALUE bdb_get(VALUE vself, VALUE vkey);
static VALUE bdb_getlist(VALUE vself, VALUE vkey);
static VALUE bdb_vnum(VALUE vself, VALUE vkey);
static VALUE bdb_vsiz(VALUE vself, VALUE vkey);
static VALUE bdb_range(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_fwmkeys(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_sync(VALUE vself);
static VALUE bdb_optimize(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_vanish(VALUE vself);
static VALUE bdb_copy(VALUE vself, VALUE vpath);
static VALUE bdb_tranbegin(VALUE vself);
static VALUE bdb_trancommit(VALUE vself);
static VALUE bdb_tranabort(VALUE vself);
static VALUE bdb_path(VALUE vself);
static VALUE bdb_rnum(VALUE vself);
static VALUE bdb_fsiz(VALUE vself);
static VALUE bdb_fetch(int argc, VALUE *argv, VALUE vself);
static VALUE bdb_check(VALUE vself, VALUE vkey);
static VALUE bdb_check_value(VALUE vself, VALUE vval);
static VALUE bdb_get_reverse(VALUE vself, VALUE vval);
static VALUE bdb_empty(VALUE vself);
static VALUE bdb_each(VALUE vself);
static VALUE bdb_each_key(VALUE vself);
static VALUE bdb_each_value(VALUE vself);
static VALUE bdb_keys(VALUE vself);
static VALUE bdb_values(VALUE vself);
static void bdbcur_init(void);
static VALUE bdbcur_initialize(VALUE vself, VALUE vbdb);
static VALUE bdbcur_first(VALUE vself);
static VALUE bdbcur_last(VALUE vself);
static VALUE bdbcur_jump(VALUE vself, VALUE vkey);
static VALUE bdbcur_prev(VALUE vself);
static VALUE bdbcur_next(VALUE vself);
static VALUE bdbcur_put(int argc, VALUE *argv, VALUE vself);
static VALUE bdbcur_out(VALUE vself);
static VALUE bdbcur_key(VALUE vself);
static VALUE bdbcur_val(VALUE vself);



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


VALUE mod_tokyocabinet;
VALUE cls_hdb;
VALUE cls_hdb_data;
VALUE cls_bdb;
VALUE cls_bdb_data;
VALUE cls_bdbcur;
VALUE cls_bdbcur_data;


int Init_tokyocabinet(void){
  mod_tokyocabinet = rb_define_module("TokyoCabinet");
  rb_define_const(mod_tokyocabinet, "VERSION", rb_str_new2(tcversion));
  hdb_init();
  bdb_init();
  bdbcur_init();
  return 0;
}



/*************************************************************************************************
 * private objects
 *************************************************************************************************/


static void hdb_init(void){
  cls_hdb = rb_define_class_under(mod_tokyocabinet, "HDB", rb_cObject);
  cls_hdb_data = rb_define_class_under(mod_tokyocabinet, "HDB_data", rb_cObject);
  rb_define_const(cls_hdb, "ESUCCESS", INT2NUM(TCESUCCESS));
  rb_define_const(cls_hdb, "ETHREAD", INT2NUM(TCETHREAD));
  rb_define_const(cls_hdb, "EINVALID", INT2NUM(TCEINVALID));
  rb_define_const(cls_hdb, "ENOFILE", INT2NUM(TCENOFILE));
  rb_define_const(cls_hdb, "ENOPERM", INT2NUM(TCENOPERM));
  rb_define_const(cls_hdb, "EMETA", INT2NUM(TCEMETA));
  rb_define_const(cls_hdb, "ERHEAD", INT2NUM(TCERHEAD));
  rb_define_const(cls_hdb, "EOPEN", INT2NUM(TCEOPEN));
  rb_define_const(cls_hdb, "ECLOSE", INT2NUM(TCECLOSE));
  rb_define_const(cls_hdb, "ETRUNC", INT2NUM(TCETRUNC));
  rb_define_const(cls_hdb, "ESYNC", INT2NUM(TCESYNC));
  rb_define_const(cls_hdb, "ESTAT", INT2NUM(TCESTAT));
  rb_define_const(cls_hdb, "ESEEK", INT2NUM(TCESEEK));
  rb_define_const(cls_hdb, "EREAD", INT2NUM(TCEREAD));
  rb_define_const(cls_hdb, "EWRITE", INT2NUM(TCEWRITE));
  rb_define_const(cls_hdb, "EMMAP", INT2NUM(TCEMMAP));
  rb_define_const(cls_hdb, "ELOCK", INT2NUM(TCELOCK));
  rb_define_const(cls_hdb, "EUNLINK", INT2NUM(TCEUNLINK));
  rb_define_const(cls_hdb, "ERENAME", INT2NUM(TCERENAME));
  rb_define_const(cls_hdb, "EMKDIR", INT2NUM(TCEMKDIR));
  rb_define_const(cls_hdb, "ERMDIR", INT2NUM(TCERMDIR));
  rb_define_const(cls_hdb, "EKEEP", INT2NUM(TCEKEEP));
  rb_define_const(cls_hdb, "ENOREC", INT2NUM(TCENOREC));
  rb_define_const(cls_hdb, "EMISC", INT2NUM(TCEMISC));
  rb_define_const(cls_hdb, "TLARGE", INT2NUM(HDBTLARGE));
  rb_define_const(cls_hdb, "TDEFLATE", INT2NUM(HDBTDEFLATE));
  rb_define_const(cls_hdb, "TTCBS", INT2NUM(HDBTTCBS));
  rb_define_const(cls_hdb, "OREADER", INT2NUM(HDBOREADER));
  rb_define_const(cls_hdb, "OWRITER", INT2NUM(HDBOWRITER));
  rb_define_const(cls_hdb, "OCREAT", INT2NUM(HDBOCREAT));
  rb_define_const(cls_hdb, "OTRUNC", INT2NUM(HDBOTRUNC));
  rb_define_const(cls_hdb, "ONOLCK", INT2NUM(HDBONOLCK));
  rb_define_const(cls_hdb, "OLCKNB", INT2NUM(HDBOLCKNB));
  rb_define_private_method(cls_hdb, "initialize", hdb_initialize, 0);
  rb_define_method(cls_hdb, "errmsg", hdb_errmsg, 1);
  rb_define_method(cls_hdb, "ecode", hdb_ecode, 0);
  rb_define_method(cls_hdb, "tune", hdb_tune, -1);
  rb_define_method(cls_hdb, "setcache", hdb_setcache, -1);
  rb_define_method(cls_hdb, "open", hdb_open, -1);
  rb_define_method(cls_hdb, "close", hdb_close, 0);
  rb_define_method(cls_hdb, "put", hdb_put, 2);
  rb_define_method(cls_hdb, "putkeep", hdb_putkeep, 2);
  rb_define_method(cls_hdb, "putcat", hdb_putcat, 2);
  rb_define_method(cls_hdb, "putasync", hdb_putasync, 2);
  rb_define_method(cls_hdb, "out", hdb_out, 1);
  rb_define_method(cls_hdb, "get", hdb_get, 1);
  rb_define_method(cls_hdb, "vsiz", hdb_vsiz, 1);
  rb_define_method(cls_hdb, "iterinit", hdb_iterinit, 0);
  rb_define_method(cls_hdb, "iternext", hdb_iternext, 0);
  rb_define_method(cls_hdb, "fwmkeys", hdb_fwmkeys, -1);
  rb_define_method(cls_hdb, "sync", hdb_sync, 0);
  rb_define_method(cls_hdb, "optimize", hdb_optimize, -1);
  rb_define_method(cls_hdb, "vanish", hdb_vanish, 0);
  rb_define_method(cls_hdb, "copy", hdb_copy, 1);
  rb_define_method(cls_hdb, "path", hdb_path, 0);
  rb_define_method(cls_hdb, "rnum", hdb_rnum, 0);
  rb_define_method(cls_hdb, "fsiz", hdb_fsiz, 0);
  rb_define_method(cls_hdb, "[]", hdb_get, 1);
  rb_define_method(cls_hdb, "[]=", hdb_put, 2);
  rb_define_method(cls_hdb, "store", hdb_put, 2);
  rb_define_method(cls_hdb, "delete", hdb_out, 1);
  rb_define_method(cls_hdb, "fetch", hdb_fetch, -1);
  rb_define_method(cls_hdb, "has_key?", hdb_check, 1);
  rb_define_method(cls_hdb, "key?", hdb_check, 1);
  rb_define_method(cls_hdb, "include?", hdb_check, 1);
  rb_define_method(cls_hdb, "member?", hdb_check, 1);
  rb_define_method(cls_hdb, "has_value?", hdb_check_value, 1);
  rb_define_method(cls_hdb, "value?", hdb_check_value, 1);
  rb_define_method(cls_hdb, "key", hdb_get_reverse, 1);
  rb_define_method(cls_hdb, "clear", hdb_vanish, 0);
  rb_define_method(cls_hdb, "size", hdb_rnum, 0);
  rb_define_method(cls_hdb, "length", hdb_rnum, 0);
  rb_define_method(cls_hdb, "empty?", hdb_empty, 0);
  rb_define_method(cls_hdb, "each", hdb_each, 0);
  rb_define_method(cls_hdb, "each_pair", hdb_each, 0);
  rb_define_method(cls_hdb, "each_key", hdb_each_key, 0);
  rb_define_method(cls_hdb, "each_value", hdb_each_value, 0);
  rb_define_method(cls_hdb, "keys", hdb_keys, 0);
  rb_define_method(cls_hdb, "values", hdb_values, 0);
}


static VALUE hdb_initialize(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  hdb = tchdbnew();
  tchdbsetmutex(hdb);
  vhdb = Data_Wrap_Struct(cls_hdb_data, 0, tchdbdel, hdb);
  rb_iv_set(vself, HDBVNDATA, vhdb);
  return Qnil;
}


static VALUE hdb_errmsg(VALUE vself, VALUE vecode){
  VALUE vhdb, vmsg;
  TCHDB *hdb;
  const char *msg;
  int ecode;
  ecode = NUM2INT(vecode);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  msg = tchdberrmsg(ecode);
  vmsg = rb_str_new2(msg);
  return vmsg;
}


static VALUE hdb_ecode(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return INT2NUM(tchdbecode(hdb));
}


static VALUE hdb_tune(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vbnum, vapow, vfpow, vopts;
  TCHDB *hdb;
  int apow, fpow, opts;
  int64_t bnum;
  rb_scan_args(argc, argv, "04", &vbnum, &vapow, &vfpow, &vopts);
  bnum = (vbnum == Qnil) ? -1 : NUM2LL(vbnum);
  apow = (vapow == Qnil) ? -1 : NUM2INT(vapow);
  fpow = (vfpow == Qnil) ? -1 : NUM2INT(vfpow);
  opts = (vopts == Qnil) ? 0 : NUM2INT(vopts);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbtune(hdb, bnum, apow, fpow, opts) ? Qtrue : Qfalse;
}


static VALUE hdb_setcache(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vrcnum;
  TCHDB *hdb;
  int rcnum;
  rb_scan_args(argc, argv, "01", &vrcnum);
  rcnum = (vrcnum == Qnil) ? -1 : NUM2INT(vrcnum);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbsetcache(hdb, rcnum) ? Qtrue : Qfalse;
}


static VALUE hdb_open(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vpath, vomode;
  TCHDB *hdb;
  const char *path;
  int omode;
  rb_scan_args(argc, argv, "11", &vpath, &vomode);
  Check_Type(vpath, T_STRING);
  path = RSTRING(vpath)->ptr;
  omode = (vomode == Qnil) ? HDBOREADER : NUM2INT(vomode);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbopen(hdb, path, omode) ? Qtrue : Qfalse;
}


static VALUE hdb_close(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbclose(hdb) ? Qtrue : Qfalse;
}


static VALUE hdb_put(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbput(hdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE hdb_putkeep(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbputkeep(hdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE hdb_putcat(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbputcat(hdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE hdb_putasync(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbputasync(hdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE hdb_out(VALUE vself, VALUE vkey){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbout(hdb, kbuf, ksiz) ? Qtrue : Qfalse;
}


static VALUE hdb_get(VALUE vself, VALUE vkey){
  VALUE vhdb, vval;
  TCHDB *hdb;
  const char *kbuf;
  char *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  if(!(vbuf = tchdbget(hdb, kbuf, ksiz, &vsiz))) return Qnil;
  vval = rb_str_new(vbuf, vsiz);
  free(vbuf);
  return vval;
}


static VALUE hdb_vsiz(VALUE vself, VALUE vkey){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return INT2NUM(tchdbvsiz(hdb, kbuf, ksiz));
}


static VALUE hdb_iterinit(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbiterinit(hdb) ? Qtrue : Qfalse;
}


static VALUE hdb_iternext(VALUE vself){
  VALUE vhdb, vval;
  TCHDB *hdb;
  char *vbuf;
  int vsiz;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  if(!(vbuf = tchdbiternext(hdb, &vsiz))) return Qnil;
  vval = rb_str_new(vbuf, vsiz);
  free(vbuf);
  return vval;
}


static VALUE hdb_fwmkeys(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vprefix, vmax, vary, vkey;
  TCHDB *hdb;
  TCLIST *keys;
  const char *pbuf, *kbuf;
  int i, psiz, max, knum, ksiz;
  rb_scan_args(argc, argv, "11", &vprefix, &vmax);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vprefix = StringValue(vprefix);
  pbuf = RSTRING(vprefix)->ptr;
  psiz = RSTRING(vprefix)->len;
  max = (vmax == Qnil) ? -1 : NUM2INT(vmax);
  keys = tchdbfwmkeys(hdb, pbuf, psiz, max);
  knum = tclistnum(keys);
  vary = rb_ary_new2(knum);
  for(i = 0; i < knum; i++){
    kbuf = tclistval(keys, i, &ksiz);
    vkey = rb_str_new(kbuf, ksiz);
    rb_ary_push(vary, vkey);
  }
  tclistdel(keys);
  return vary;
}


static VALUE hdb_sync(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbsync(hdb) ? Qtrue : Qfalse;
}


static VALUE hdb_optimize(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vbnum, vapow, vfpow, vopts;
  TCHDB *hdb;
  int apow, fpow, opts;
  int64_t bnum;
  rb_scan_args(argc, argv, "04", &vbnum, &vapow, &vfpow, &vopts);
  bnum = (vbnum == Qnil) ? -1 : NUM2LL(vbnum);
  apow = (vapow == Qnil) ? -1 : NUM2INT(vapow);
  fpow = (vfpow == Qnil) ? -1 : NUM2INT(vfpow);
  opts = (vopts == Qnil) ? 0xff : NUM2INT(vopts);
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdboptimize(hdb, bnum, apow, fpow, opts) ? Qtrue : Qfalse;
}


static VALUE hdb_vanish(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbvanish(hdb) ? Qtrue : Qfalse;
}


static VALUE hdb_copy(VALUE vself, VALUE vpath){
  VALUE vhdb;
  TCHDB *hdb;
  const char *path;
  Check_Type(vpath, T_STRING);
  path = RSTRING(vpath)->ptr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbcopy(hdb, path) ? Qtrue : Qfalse;
}


static VALUE hdb_path(VALUE vself){
  VALUE vhdb, vpath;
  TCHDB *hdb;
  const char *path;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  if(!(path = tchdbpath(hdb))) return Qnil;
  vpath = rb_str_new2(path);
  return vpath;
}


static VALUE hdb_rnum(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return LL2NUM(tchdbrnum(hdb));
}


static VALUE hdb_fsiz(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return LL2NUM(tchdbfsiz(hdb));
}


static VALUE hdb_fetch(int argc, VALUE *argv, VALUE vself){
  VALUE vhdb, vkey, vdef, vval;
  TCHDB *hdb;
  const char *kbuf;
  char *vbuf;
  int ksiz, vsiz;
  rb_scan_args(argc, argv, "11", &vkey, &vdef);
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  if((vbuf = tchdbget(hdb, kbuf, ksiz, &vsiz)) != NULL){
    vval = rb_str_new(vbuf, vsiz);
    free(vbuf);
  } else {
    vval = vdef;
  }
  return vval;
}


static VALUE hdb_check(VALUE vself, VALUE vkey){
  VALUE vhdb;
  TCHDB *hdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbvsiz(hdb, kbuf, ksiz) >= 0 ? Qtrue : Qfalse;
}


static VALUE hdb_check_value(VALUE vself, VALUE vval){
  VALUE vhdb;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  const char *vbuf;
  int vsiz, hit;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  hit = 0;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    if(tcxstrsize(vxstr) == vsiz && memcmp(tcxstrptr(vxstr), vbuf, vsiz) == 0){
      hit = 1;
      break;
    }
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return hit ? Qtrue : Qfalse;
}


static VALUE hdb_get_reverse(VALUE vself, VALUE vval){
  VALUE vhdb, vrv;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  const char *vbuf;
  int vsiz;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vrv = Qnil;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    if(tcxstrsize(vxstr) == vsiz && memcmp(tcxstrptr(vxstr), vbuf, vsiz) == 0){
      vrv = rb_str_new(tcxstrptr(kxstr), tcxstrsize(kxstr));
      break;
    }
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vrv;
}


static VALUE hdb_empty(VALUE vself){
  VALUE vhdb;
  TCHDB *hdb;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  return tchdbrnum(hdb) < 1 ? Qtrue : Qfalse;
}


static VALUE hdb_each(VALUE vself){
  VALUE vhdb, vrv, vkey, vval, vary;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vrv = Qnil;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    vkey = rb_str_new(tcxstrptr(kxstr), tcxstrsize(kxstr));
    vval = rb_str_new(tcxstrptr(vxstr), tcxstrsize(vxstr));
    vary = rb_ary_new3(2, vkey, vval);
    vrv = rb_yield(vary);
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vrv;
}


static VALUE hdb_each_key(VALUE vself){
  VALUE vhdb, vrv, vkey;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vrv = Qnil;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    vkey = rb_str_new(tcxstrptr(kxstr), tcxstrsize(kxstr));
    vrv = rb_yield(vkey);
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vrv;
}


static VALUE hdb_each_value(VALUE vself){
  VALUE vhdb, vrv, vval;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vrv = Qnil;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    vval = rb_str_new(tcxstrptr(vxstr), tcxstrsize(vxstr));
    vrv = rb_yield(vval);
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vrv;
}


static VALUE hdb_keys(VALUE vself){
  VALUE vhdb, vkey, vary;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vary = rb_ary_new2(tchdbrnum(hdb));
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    vkey = rb_str_new(tcxstrptr(kxstr), tcxstrsize(kxstr));
    rb_ary_push(vary, vkey);
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vary;
}


static VALUE hdb_values(VALUE vself){
  VALUE vhdb, vval, vary;
  TCHDB *hdb;
  TCXSTR *kxstr, *vxstr;
  vhdb = rb_iv_get(vself, HDBVNDATA);
  Data_Get_Struct(vhdb, TCHDB, hdb);
  vary = rb_ary_new2(tchdbrnum(hdb));
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  tchdbiterinit(hdb);
  while(tchdbiternext3(hdb, kxstr, vxstr)){
    vval = rb_str_new(tcxstrptr(vxstr), tcxstrsize(vxstr));
    rb_ary_push(vary, vval);
  }
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vary;
}


static void bdb_init(void){
  cls_bdb = rb_define_class_under(mod_tokyocabinet, "BDB", rb_cObject);
  cls_bdb_data = rb_define_class_under(mod_tokyocabinet, "BDB_data", rb_cObject);
  rb_define_const(cls_bdb, "ESUCCESS", INT2NUM(TCESUCCESS));
  rb_define_const(cls_bdb, "ETHREAD", INT2NUM(TCETHREAD));
  rb_define_const(cls_bdb, "EINVALID", INT2NUM(TCEINVALID));
  rb_define_const(cls_bdb, "ENOFILE", INT2NUM(TCENOFILE));
  rb_define_const(cls_bdb, "ENOPERM", INT2NUM(TCENOPERM));
  rb_define_const(cls_bdb, "EMETA", INT2NUM(TCEMETA));
  rb_define_const(cls_bdb, "ERHEAD", INT2NUM(TCERHEAD));
  rb_define_const(cls_bdb, "EOPEN", INT2NUM(TCEOPEN));
  rb_define_const(cls_bdb, "ECLOSE", INT2NUM(TCECLOSE));
  rb_define_const(cls_bdb, "ETRUNC", INT2NUM(TCETRUNC));
  rb_define_const(cls_bdb, "ESYNC", INT2NUM(TCESYNC));
  rb_define_const(cls_bdb, "ESTAT", INT2NUM(TCESTAT));
  rb_define_const(cls_bdb, "ESEEK", INT2NUM(TCESEEK));
  rb_define_const(cls_bdb, "EREAD", INT2NUM(TCEREAD));
  rb_define_const(cls_bdb, "EWRITE", INT2NUM(TCEWRITE));
  rb_define_const(cls_bdb, "EMMAP", INT2NUM(TCEMMAP));
  rb_define_const(cls_bdb, "ELOCK", INT2NUM(TCELOCK));
  rb_define_const(cls_bdb, "EUNLINK", INT2NUM(TCEUNLINK));
  rb_define_const(cls_bdb, "ERENAME", INT2NUM(TCERENAME));
  rb_define_const(cls_bdb, "EMKDIR", INT2NUM(TCEMKDIR));
  rb_define_const(cls_bdb, "ERMDIR", INT2NUM(TCERMDIR));
  rb_define_const(cls_bdb, "EKEEP", INT2NUM(TCEKEEP));
  rb_define_const(cls_bdb, "ENOREC", INT2NUM(TCENOREC));
  rb_define_const(cls_bdb, "EMISC", INT2NUM(TCEMISC));
  rb_define_const(cls_bdb, "TLARGE", INT2NUM(BDBTLARGE));
  rb_define_const(cls_bdb, "TDEFLATE", INT2NUM(BDBTDEFLATE));
  rb_define_const(cls_bdb, "TTCBS", INT2NUM(BDBTTCBS));
  rb_define_const(cls_bdb, "OREADER", INT2NUM(BDBOREADER));
  rb_define_const(cls_bdb, "OWRITER", INT2NUM(BDBOWRITER));
  rb_define_const(cls_bdb, "OCREAT", INT2NUM(BDBOCREAT));
  rb_define_const(cls_bdb, "OTRUNC", INT2NUM(BDBOTRUNC));
  rb_define_const(cls_bdb, "ONOLCK", INT2NUM(BDBONOLCK));
  rb_define_const(cls_bdb, "OLCKNB", INT2NUM(BDBOLCKNB));
  rb_define_private_method(cls_bdb, "initialize", bdb_initialize, 0);
  rb_define_method(cls_bdb, "errmsg", bdb_errmsg, 1);
  rb_define_method(cls_bdb, "ecode", bdb_ecode, 0);
  rb_define_method(cls_bdb, "errmsg", bdb_errmsg, 1);
  rb_define_method(cls_bdb, "ecode", bdb_ecode, 0);
  rb_define_method(cls_bdb, "tune", bdb_tune, -1);
  rb_define_method(cls_bdb, "setcache", bdb_setcache, -1);
  rb_define_method(cls_bdb, "open", bdb_open, -1);
  rb_define_method(cls_bdb, "close", bdb_close, 0);
  rb_define_method(cls_bdb, "put", bdb_put, 2);
  rb_define_method(cls_bdb, "putkeep", bdb_putkeep, 2);
  rb_define_method(cls_bdb, "putcat", bdb_putcat, 2);
  rb_define_method(cls_bdb, "putdup", bdb_putdup, 2);
  rb_define_method(cls_bdb, "putlist", bdb_putlist, 2);
  rb_define_method(cls_bdb, "out", bdb_out, 1);
  rb_define_method(cls_bdb, "outlist", bdb_outlist, 1);
  rb_define_method(cls_bdb, "get", bdb_get, 1);
  rb_define_method(cls_bdb, "getlist", bdb_getlist, 1);
  rb_define_method(cls_bdb, "vnum", bdb_vnum, 1);
  rb_define_method(cls_bdb, "vsiz", bdb_vsiz, 1);
  rb_define_method(cls_bdb, "range", bdb_range, -1);
  rb_define_method(cls_bdb, "fwmkeys", bdb_fwmkeys, -1);
  rb_define_method(cls_bdb, "sync", bdb_sync, 0);
  rb_define_method(cls_bdb, "optimize", bdb_optimize, -1);
  rb_define_method(cls_bdb, "vanish", bdb_vanish, 0);
  rb_define_method(cls_bdb, "copy", bdb_copy, 1);
  rb_define_method(cls_bdb, "tranbegin", bdb_tranbegin, 0);
  rb_define_method(cls_bdb, "trancommit", bdb_trancommit, 0);
  rb_define_method(cls_bdb, "tranabort", bdb_tranabort, 0);
  rb_define_method(cls_bdb, "path", bdb_path, 0);
  rb_define_method(cls_bdb, "rnum", bdb_rnum, 0);
  rb_define_method(cls_bdb, "fsiz", bdb_fsiz, 0);
  rb_define_method(cls_bdb, "[]", bdb_get, 1);
  rb_define_method(cls_bdb, "[]=", bdb_put, 2);
  rb_define_method(cls_bdb, "store", bdb_put, 2);
  rb_define_method(cls_bdb, "delete", bdb_out, 1);
  rb_define_method(cls_bdb, "fetch", bdb_fetch, -1);
  rb_define_method(cls_bdb, "has_key?", bdb_check, 1);
  rb_define_method(cls_bdb, "key?", bdb_check, 1);
  rb_define_method(cls_bdb, "include?", bdb_check, 1);
  rb_define_method(cls_bdb, "member?", bdb_check, 1);
  rb_define_method(cls_bdb, "has_value?", bdb_check_value, 1);
  rb_define_method(cls_bdb, "value?", bdb_check_value, 1);
  rb_define_method(cls_bdb, "key", bdb_get_reverse, 1);
  rb_define_method(cls_bdb, "clear", bdb_vanish, 0);
  rb_define_method(cls_bdb, "size", bdb_rnum, 0);
  rb_define_method(cls_bdb, "length", bdb_rnum, 0);
  rb_define_method(cls_bdb, "empty?", bdb_empty, 0);
  rb_define_method(cls_bdb, "each", bdb_each, 0);
  rb_define_method(cls_bdb, "each_pair", bdb_each, 0);
  rb_define_method(cls_bdb, "each_key", bdb_each_key, 0);
  rb_define_method(cls_bdb, "each_value", bdb_each_value, 0);
  rb_define_method(cls_bdb, "keys", bdb_keys, 0);
  rb_define_method(cls_bdb, "values", bdb_values, 0);
}


static VALUE bdb_initialize(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  bdb = tcbdbnew();
  tcbdbsetmutex(bdb);
  vbdb = Data_Wrap_Struct(cls_bdb_data, 0, tcbdbdel, bdb);
  rb_iv_set(vself, BDBVNDATA, vbdb);
  return Qnil;
}


static VALUE bdb_errmsg(VALUE vself, VALUE vecode){
  VALUE vbdb, vmsg;
  TCBDB *bdb;
  const char *msg;
  int ecode;
  ecode = NUM2INT(vecode);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  msg = tcbdberrmsg(ecode);
  vmsg = rb_str_new2(msg);
  return vmsg;
}


static VALUE bdb_ecode(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return INT2NUM(tcbdbecode(bdb));
}


static VALUE bdb_tune(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vlmemb, vnmemb, vbnum, vapow, vfpow, vopts;
  TCBDB *bdb;
  int lmemb, nmemb, apow, fpow, opts;
  int64_t bnum;
  rb_scan_args(argc, argv, "06", &vlmemb, &vnmemb, &vbnum, &vapow, &vfpow, &vopts);
  lmemb = (vlmemb == Qnil) ? -1 : NUM2INT(vlmemb);
  nmemb = (vnmemb == Qnil) ? -1 : NUM2INT(vnmemb);
  bnum = (vbnum == Qnil) ? -1 : NUM2LL(vbnum);
  apow = (vapow == Qnil) ? -1 : NUM2INT(vapow);
  fpow = (vfpow == Qnil) ? -1 : NUM2INT(vfpow);
  opts = (vopts == Qnil) ? 0 : NUM2INT(vopts);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbtune(bdb, lmemb, nmemb, bnum, apow, fpow, opts) ? Qtrue : Qfalse;
}


static VALUE bdb_setcache(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vlcnum, vncnum;
  TCBDB *bdb;
  int lcnum, ncnum;
  rb_scan_args(argc, argv, "02", &vlcnum, &vncnum);
  lcnum = (vlcnum == Qnil) ? -1 : NUM2INT(vlcnum);
  ncnum = (vncnum == Qnil) ? -1 : NUM2INT(vncnum);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbsetcache(bdb, lcnum, ncnum);
}


static VALUE bdb_open(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vpath, vomode;
  TCBDB *bdb;
  const char *path;
  int omode;
  rb_scan_args(argc, argv, "11", &vpath, &vomode);
  Check_Type(vpath, T_STRING);
  path = RSTRING(vpath)->ptr;
  omode = (vomode == Qnil) ? BDBOREADER : NUM2INT(vomode);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbopen(bdb, path, omode) ? Qtrue : Qfalse;
}


static VALUE bdb_close(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbclose(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_put(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbput(bdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE bdb_putkeep(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbputkeep(bdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE bdb_putcat(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbputcat(bdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE bdb_putdup(VALUE vself, VALUE vkey, VALUE vval){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbputdup(bdb, kbuf, ksiz, vbuf, vsiz) ? Qtrue : Qfalse;
}


static VALUE bdb_putlist(VALUE vself, VALUE vkey, VALUE vvals){
  VALUE vbdb, vval;
  TCBDB *bdb;
  TCLIST *tvals;
  const char *kbuf;
  int i, num, ksiz;
  bool err;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  Check_Type(vvals, T_ARRAY);
  tvals = tclistnew();
  num = RARRAY(vvals)->len;
  for(i = 0; i < num; i++){
    vval = rb_ary_entry(vvals, i);
    vval = StringValue(vval);
    tclistpush(tvals, RSTRING(vval)->ptr, RSTRING(vval)->len);
  }
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  err = false;
  if(!tcbdbputdup3(bdb, kbuf, ksiz, tvals)) err = true;
  tclistdel(tvals);
  return err ? Qfalse : Qtrue;
}


static VALUE bdb_out(VALUE vself, VALUE vkey){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbout(bdb, kbuf, ksiz) ? Qtrue : Qfalse;
}


static VALUE bdb_outlist(VALUE vself, VALUE vkey){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbout3(bdb, kbuf, ksiz) ? Qtrue : Qfalse;
}


static VALUE bdb_get(VALUE vself, VALUE vkey){
  VALUE vbdb, vval;
  TCBDB *bdb;
  const char *kbuf, *vbuf;
  int ksiz, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  if(!(vbuf = tcbdbget3(bdb, kbuf, ksiz, &vsiz))) return Qnil;
  vval = rb_str_new(vbuf, vsiz);
  return vval;
}


static VALUE bdb_getlist(VALUE vself, VALUE vkey){
  VALUE vbdb, vary, vval;
  TCBDB *bdb;
  TCLIST *vals;
  const char *kbuf, *vbuf;
  int i, ksiz, vnum, vsiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  if(!(vals = tcbdbget4(bdb, kbuf, ksiz))) return Qnil;
  vnum = tclistnum(vals);
  vary = rb_ary_new2(vnum);
  for(i = 0; i < vnum; i++){
    vbuf = tclistval(vals, i, &vsiz);
    vval = rb_str_new(vbuf, vsiz);
    rb_ary_push(vary, vval);
  }
  tclistdel(vals);
  return vary;
}


static VALUE bdb_vnum(VALUE vself, VALUE vkey){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return INT2NUM(tcbdbvnum(bdb, kbuf, ksiz));
}


static VALUE bdb_vsiz(VALUE vself, VALUE vkey){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return INT2NUM(tcbdbvsiz(bdb, kbuf, ksiz));
}


static VALUE bdb_range(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vbkey, vbinc, vekey, veinc, vmax, vary, vkey;
  TCBDB *bdb;
  TCLIST *keys;
  const char *bkbuf, *ekbuf, *kbuf;
  int i, bksiz, eksiz, max, knum, ksiz;
  bool binc, einc;
  rb_scan_args(argc, argv, "05", &vbkey, &vbinc, &vekey, &veinc, &vmax);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  if(vbkey != Qnil){
    vbkey = StringValue(vbkey);
    bkbuf = RSTRING(vbkey)->ptr;
    bksiz = RSTRING(vbkey)->len;
  } else {
    bkbuf = NULL;
    bksiz = -1;
  }
  binc = (vbinc != Qnil && vbinc != Qfalse);
  if(vekey != Qnil){
    vekey = StringValue(vekey);
    ekbuf = RSTRING(vekey)->ptr;
    eksiz = RSTRING(vekey)->len;
  } else {
    ekbuf = NULL;
    eksiz = -1;
  }
  einc = (veinc != Qnil && veinc != Qfalse);
  max = (vmax == Qnil) ? -1 : NUM2INT(vmax);
  keys = tcbdbrange(bdb, bkbuf, bksiz, binc, ekbuf, eksiz, einc, max);
  knum = tclistnum(keys);
  vary = rb_ary_new2(knum);
  for(i = 0; i < knum; i++){
    kbuf = tclistval(keys, i, &ksiz);
    vkey = rb_str_new(kbuf, ksiz);
    rb_ary_push(vary, vkey);
  }
  tclistdel(keys);
  return vary;
}


static VALUE bdb_fwmkeys(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vprefix, vmax, vary, vkey;
  TCBDB *bdb;
  TCLIST *keys;
  const char *pbuf, *kbuf;
  int i, psiz, max, knum, ksiz;
  rb_scan_args(argc, argv, "11", &vprefix, &vmax);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vprefix = StringValue(vprefix);
  pbuf = RSTRING(vprefix)->ptr;
  psiz = RSTRING(vprefix)->len;
  max = (vmax == Qnil) ? -1 : NUM2INT(vmax);
  keys = tcbdbfwmkeys(bdb, pbuf, psiz, max);
  knum = tclistnum(keys);
  vary = rb_ary_new2(knum);
  for(i = 0; i < knum; i++){
    kbuf = tclistval(keys, i, &ksiz);
    vkey = rb_str_new(kbuf, ksiz);
    rb_ary_push(vary, vkey);
  }
  tclistdel(keys);
  return vary;
}


static VALUE bdb_sync(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbsync(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_optimize(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vlmemb, vnmemb, vbnum, vapow, vfpow, vopts;
  TCBDB *bdb;
  int lmemb, nmemb, apow, fpow, opts;
  int64_t bnum;
  rb_scan_args(argc, argv, "06", &vlmemb, &vnmemb, &vbnum, &vapow, &vfpow, &vopts);
  lmemb = (vlmemb == Qnil) ? -1 : NUM2INT(vlmemb);
  nmemb = (vnmemb == Qnil) ? -1 : NUM2INT(vnmemb);
  bnum = (vbnum == Qnil) ? -1 : NUM2LL(vbnum);
  apow = (vapow == Qnil) ? -1 : NUM2INT(vapow);
  fpow = (vfpow == Qnil) ? -1 : NUM2INT(vfpow);
  opts = (vopts == Qnil) ? 0xff : NUM2INT(vopts);
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdboptimize(bdb, lmemb, nmemb, bnum, apow, fpow, opts) ? Qtrue : Qfalse;
}


static VALUE bdb_vanish(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbvanish(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_copy(VALUE vself, VALUE vpath){
  VALUE vbdb;
  TCBDB *bdb;
  const char *path;
  Check_Type(vpath, T_STRING);
  path = RSTRING(vpath)->ptr;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbcopy(bdb, path) ? Qtrue : Qfalse;
}


static VALUE bdb_tranbegin(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbtranbegin(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_trancommit(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbtrancommit(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_tranabort(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbtranabort(bdb) ? Qtrue : Qfalse;
}


static VALUE bdb_path(VALUE vself){
  VALUE vbdb, vpath;
  TCBDB *bdb;
  const char *path;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  if(!(path = tcbdbpath(bdb))) return Qnil;
  vpath = rb_str_new2(path);
  return vpath;
}


static VALUE bdb_rnum(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return LL2NUM(tcbdbrnum(bdb));
}


static VALUE bdb_fsiz(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return LL2NUM(tcbdbfsiz(bdb));
}


static VALUE bdb_fetch(int argc, VALUE *argv, VALUE vself){
  VALUE vbdb, vkey, vdef, vval;
  TCBDB *bdb;
  const char *kbuf;
  char *vbuf;
  int ksiz, vsiz;
  rb_scan_args(argc, argv, "11", &vkey, &vdef);
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  if((vbuf = tcbdbget(bdb, kbuf, ksiz, &vsiz)) != NULL){
    vval = rb_str_new(vbuf, vsiz);
    free(vbuf);
  } else {
    vval = vdef;
  }
  return vval;
}


static VALUE bdb_check(VALUE vself, VALUE vkey){
  VALUE vbdb;
  TCBDB *bdb;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbvsiz(bdb, kbuf, ksiz) >= 0 ? Qtrue : Qfalse;
}


static VALUE bdb_check_value(VALUE vself, VALUE vval){
  VALUE vbdb;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *vbuf, *tvbuf;
  int vsiz, tvsiz, hit;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  hit = 0;
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((tvbuf = tcbdbcurval3(cur, &tvsiz)) != NULL){
    if(tvsiz == vsiz && memcmp(tvbuf, vbuf, vsiz) == 0){
      hit = 1;
      break;
    }
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return hit ? Qtrue : Qfalse;
}


static VALUE bdb_get_reverse(VALUE vself, VALUE vval){
  VALUE vbdb, vrv;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *vbuf, *tvbuf, *tkbuf;
  int vsiz, tvsiz, tksiz;
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vrv = Qnil;
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((tvbuf = tcbdbcurval3(cur, &tvsiz)) != NULL){
    if(tvsiz == vsiz && memcmp(tvbuf, vbuf, vsiz) == 0){
      if((tkbuf = tcbdbcurkey3(cur, &tksiz)) != NULL)
        vrv = rb_str_new(tkbuf, tksiz);
      break;
    }
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return vrv;
}


static VALUE bdb_empty(VALUE vself){
  VALUE vbdb;
  TCBDB *bdb;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  return tcbdbrnum(bdb) < 1 ? Qtrue : Qfalse;
}


static VALUE bdb_each(VALUE vself){
  VALUE vbdb, vrv, vkey, vval, vary;
  TCBDB *bdb;
  BDBCUR *cur;
  TCXSTR *kxstr, *vxstr;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vrv = Qnil;
  kxstr = tcxstrnew();
  vxstr = tcxstrnew();
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while(tcbdbcurrec(cur, kxstr, vxstr)){
    vkey = rb_str_new(tcxstrptr(kxstr), tcxstrsize(kxstr));
    vval = rb_str_new(tcxstrptr(vxstr), tcxstrsize(vxstr));
    vary = rb_ary_new3(2, vkey, vval);
    vrv = rb_yield(vary);
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  tcxstrdel(vxstr);
  tcxstrdel(kxstr);
  return vrv;
}


static VALUE bdb_each_key(VALUE vself){
  VALUE vbdb, vrv, vkey;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *kbuf;
  int ksiz;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vrv = Qnil;
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((kbuf = tcbdbcurkey3(cur, &ksiz)) != NULL){
    vkey = rb_str_new(kbuf, ksiz);
    vrv = rb_yield(vkey);
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return vrv;
}


static VALUE bdb_each_value(VALUE vself){
  VALUE vbdb, vrv, vval;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *vbuf;
  int vsiz;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vrv = Qnil;
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((vbuf = tcbdbcurval3(cur, &vsiz)) != NULL){
    vval = rb_str_new(vbuf, vsiz);
    vrv = rb_yield(vval);
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return vrv;
}


static VALUE bdb_keys(VALUE vself){
  VALUE vbdb, vkey, vary;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *kbuf;
  int ksiz;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vary = rb_ary_new2(tcbdbrnum(bdb));
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((kbuf = tcbdbcurkey3(cur, &ksiz)) != NULL){
    vkey = rb_str_new(kbuf, ksiz);
    rb_ary_push(vary, vkey);
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return vary;
}


static VALUE bdb_values(VALUE vself){
  VALUE vbdb, vval, vary;
  TCBDB *bdb;
  BDBCUR *cur;
  const char *vbuf;
  int vsiz;
  vbdb = rb_iv_get(vself, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  vary = rb_ary_new2(tcbdbrnum(bdb));
  cur = tcbdbcurnew(bdb);
  tcbdbcurfirst(cur);
  while((vbuf = tcbdbcurval3(cur, &vsiz)) != NULL){
    vval = rb_str_new(vbuf, vsiz);
    rb_ary_push(vary, vval);
    tcbdbcurnext(cur);
  }
  tcbdbcurdel(cur);
  return vary;
}


static void bdbcur_init(void){
  cls_bdbcur = rb_define_class_under(mod_tokyocabinet, "BDBCUR", rb_cObject);
  cls_bdbcur_data = rb_define_class_under(mod_tokyocabinet, "BDBCUR_data", rb_cObject);
  rb_define_const(cls_bdbcur, "CPCURRENT", INT2NUM(BDBCPCURRENT));
  rb_define_const(cls_bdbcur, "CPBEFORE", INT2NUM(BDBCPBEFORE));
  rb_define_const(cls_bdbcur, "CPAFTER", INT2NUM(BDBCPAFTER));
  rb_define_private_method(cls_bdbcur, "initialize", bdbcur_initialize, 1);
  rb_define_method(cls_bdbcur, "first", bdbcur_first, 0);
  rb_define_method(cls_bdbcur, "last", bdbcur_last, 0);
  rb_define_method(cls_bdbcur, "jump", bdbcur_jump, 1);
  rb_define_method(cls_bdbcur, "prev", bdbcur_prev, 0);
  rb_define_method(cls_bdbcur, "next", bdbcur_next, 0);
  rb_define_method(cls_bdbcur, "put", bdbcur_put, -1);
  rb_define_method(cls_bdbcur, "out", bdbcur_out, 0);
  rb_define_method(cls_bdbcur, "key", bdbcur_key, 0);
  rb_define_method(cls_bdbcur, "val", bdbcur_val, 0);
}


static VALUE bdbcur_initialize(VALUE vself, VALUE vbdb){
  VALUE vcur;
  TCBDB *bdb;
  BDBCUR *cur;
  Check_Type(vbdb, T_OBJECT);
  vbdb = rb_iv_get(vbdb, BDBVNDATA);
  Data_Get_Struct(vbdb, TCBDB, bdb);
  cur = tcbdbcurnew(bdb);
  vcur = Data_Wrap_Struct(cls_bdbcur_data, 0, tcbdbcurdel, cur);
  rb_iv_set(vself, BDBCURVNDATA, vcur);
  return Qnil;
}


static VALUE bdbcur_first(VALUE vself){
  VALUE vcur;
  BDBCUR *cur;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurfirst(cur) ? Qtrue : Qfalse;
}


static VALUE bdbcur_last(VALUE vself){
  VALUE vcur;
  BDBCUR *cur;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurlast(cur) ? Qtrue : Qfalse;
}


static VALUE bdbcur_jump(VALUE vself, VALUE vkey){
  VALUE vcur;
  BDBCUR *cur;
  const char *kbuf;
  int ksiz;
  vkey = StringValue(vkey);
  kbuf = RSTRING(vkey)->ptr;
  ksiz = RSTRING(vkey)->len;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurjump(cur, kbuf, ksiz) ? Qtrue : Qfalse;
}


static VALUE bdbcur_prev(VALUE vself){
  VALUE vcur;
  BDBCUR *cur;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurprev(cur) ? Qtrue : Qfalse;
}


static VALUE bdbcur_next(VALUE vself){
  VALUE vcur;
  BDBCUR *cur;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurnext(cur) ? Qtrue : Qfalse;
}


static VALUE bdbcur_put(int argc, VALUE *argv, VALUE vself){
  VALUE vcur, vval, vcpmode;
  BDBCUR *cur;
  const char *vbuf;
  int vsiz, cpmode;
  rb_scan_args(argc, argv, "11", &vval, &vcpmode);
  vval = StringValue(vval);
  vbuf = RSTRING(vval)->ptr;
  vsiz = RSTRING(vval)->len;
  cpmode = (vcpmode == Qnil) ? BDBCPCURRENT : NUM2INT(vcpmode);
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurput(cur, vbuf, vsiz, cpmode) ? Qtrue : Qfalse;
}


static VALUE bdbcur_out(VALUE vself){
  VALUE vcur;
  BDBCUR *cur;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  return tcbdbcurout(cur) ? Qtrue : Qfalse;
}


static VALUE bdbcur_key(VALUE vself){
  VALUE vcur, vkey;
  BDBCUR *cur;
  char *kbuf;
  int ksiz;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  if(!(kbuf = tcbdbcurkey(cur, &ksiz))) return Qnil;
  vkey = rb_str_new(kbuf, ksiz);
  free(kbuf);
  return vkey;
}


static VALUE bdbcur_val(VALUE vself){
  VALUE vcur, vval;
  BDBCUR *cur;
  char *vbuf;
  int vsiz;
  vcur = rb_iv_get(vself, BDBCURVNDATA);
  Data_Get_Struct(vcur, BDBCUR, cur);
  if(!(vbuf = tcbdbcurval(cur, &vsiz))) return Qnil;
  vval = rb_str_new(vbuf, vsiz);
  free(vbuf);
  return vval;
}



/* END OF FILE */
