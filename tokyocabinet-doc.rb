#--
# Ruby binding of Tokyo Cabinet
#                                                       Copyright (C) 2006-2007 Mikio Hirabayashi
#  This file is part of Tokyo Cabinet.
#  Tokyo Cabinet is free software; you can redistribute it and/or modify it under the terms of
#  the GNU Lesser General Public License as published by the Free Software Foundation; either
#  version 2.1 of the License or any later version.  Tokyo Cabinet is distributed in the hope
#  that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
#  License for more details.
#  You should have received a copy of the GNU Lesser General Public License along with Tokyo
#  Cabinet; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
#  Boston, MA 02111-1307 USA.
#++
#:include:overview.rd


module TokyoCabinet
  # the version information
  VERSION = "x.y.z"
  # Hash database is a file containing a hash table and is handled with the hash database API.  Before operations to store or retrieve records, it is necessary to open a database file and connect the hash database object to it.  To avoid data missing or corruption, it is important to close every database file when it is no longer in use.
  # Except for the interface below, methods compatible with the `Hash' class are also provided; `[]', `[]=', `store', `delete', `fetch', `has_key?', `has_value?', `key', `clear', `size', `empty?', `each', `each_key', `each_value', `keys', and `value'.
  class HDB
    # error code: success
    TCESUCCESS = 0
    # error code: threading error
    TCETHREAD = 1
    # error code: invalid operation
    TCEINVALID = 2
    # error code: file not found
    TCENOFILE = 3
    # error code: no permission
    TCENOPERM = 4
    # error code: invalid meta data
    TCEMETA = 5
    # error code: invalid record header
    TCERHEAD = 6
    # error code: open error
    TCEOPEN = 7
    # error code: close error
    TCECLOSE = 8
    # error code: trunc error
    TCETRUNC = 9
    # error code: sync error
    TCESYNC = 10
    # error code: stat error
    TCESTAT = 11
    # error code: seek error
    TCESEEK = 12
    # error code: read error
    TCEREAD = 13
    # error code: write error
    TCEWRITE = 14
    # error code: mmap error
    TCEMMAP = 15
    # error code: lock error
    TCELOCK = 16
    # error code: unlink error
    TCEUNLINK = 17
    # error code: rename error
    TCERENAME = 18
    # error code: mkdir error
    TCEMKDIR = 19
    # error code: rmdir error
    TCERMDIR = 20
    # error code: existing record
    TCEKEEP = 21
    # error code: no record found
    TCENOREC = 22
    # error code: miscellaneous error
    TCEMISC = 9999
    # tuning option: use 64-bit bucket array
    TLARGE = 1 << 0
    # tuning option: compress each record with Deflate
    TDEFLATE = 1 << 1
    # tuning option: compress each record with TCBS
    TTCBS = 1 << 2
    # open mode: open as a reader
    OREADER = 1 << 0
    # open mode: open as a writer
    OWRITER = 1 << 1
    # open mode: writer creating
    OCREAT = 1 << 2
    # open mode: writer truncating
    OTRUNC = 1 << 3
    # open mode: open without locking
    ONOLCK = 1 << 4
    # open mode: lock without blocking
    OLCKNB = 1 << 5
    # Create a hash database object.
    # The return value is the new hash database object.
    def initialize()
      # (native code)
    end
    # Get the message string corresponding to an error code.
    # `<i>ecode</i>' specifies the error code.
    # The return value is the message string of the error code.
    def errmsg(ecode)
      # (native code)
    end
    # Get the last happened error code.
    # The return value is the last happened error code.
    def ecode()
      # (native code)
    end
    # Set the tuning parameters.
    # `<i>bnum</i>' specifies the number of elements of the bucket array.  If it is not defined or not more than 0, the default value is specified.  The default value is 16381.  Suggested size of the bucket array is about from 0.5 to 4 times of the number of all records to be stored.
    # `<i>apow</i>' specifies the size of record alignment by power of 2.  If it is not defined or negative, the default value is specified.  The default value is 4 standing for 2^4=16.
    # `<i>fpow</i>' specifies the maximum number of elements of the free block pool by power of 2.  If it is not defined or negative, the default value is specified.  The default value is 10 standing for 2^10=1024.
    # `<i>opts</i>' specifies options by bitwise or: `TokyoCabinet::HDB::TLARGE' specifies that the size of the database can be larger than 2GB by using 64-bit bucket array, `TokyoCabinet::HDB::TDEFLATE' specifies that each record is compressed with Deflate encoding, `TokyoCabinet::HDB::TTCBS' specifies that each record is compressed with TCBS encoding.  If it is not defined, no option is specified.
    # If successful, the return value is true, else, it is false.  Note that the tuning parameters of the database should be set before the database is opened.
    def tune(bnum, apow, fpow, opts)
      # (native code)
    end
    # Open a database file.
    # `<i>path</i>' specifies the path of the database file.
    # `<i>omode</i>' specifies the connection mode: `TokyoCabinet::HDB::OWRITER' as a writer, `TokyoCabinet::HDB::OREADER' as a reader.  If the mode is `TokyoCabinet::HDB::OWRITER', the following may be added by bitwise or: `TokyoCabinet::HDB::OCREAT', which means it creates a new database if not exist, `TokyoCabinet::HDB::OTRUNC', which means it creates a new database regardless if one exists.  Both of `TokyoCabinet::HDB::OREADER' and `TokyoCabinet::HDB::OWRITER' can be added to by bitwise or: `TokyoCabinet::HDB::ONOLCK', which means it opens the database file without file locking, or `TokyoCabinet::HDB::OLCKNB', which means locking is performed without blocking.  If it is not defined, `TokyoCabinet::HDB::OREADER' is specified.
    # If successful, the return value is true, else, it is false.
    def open(path, omode)
      # (native code)
    end
    # Close the database file.
    # If successful, the return value is true, else, it is false.
    # Update of a database is assured to be written when the database is closed.  If a writer opens a database but does not close it appropriately, the database will be broken.
    def close()
      # (native code)
    end
    # Store a record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, it is overwritten.
    def put(key, value)
      # (native code)
    end
    # Store a new record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, this function has no effect.
    def putkeep(key, value)
      # (native code)
    end
    # Concatenate a value at the end of the existing record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If there is no corresponding record, a new record is created.
    def putcat(key, value)
      # (native code)
    end
    # Store a record in asynchronous fashion.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, it is overwritten.  Records passed to this function are accumulated into the inner buffer and wrote into the file at a blast.
    def putasync(key, value)
      # (native code)
    end
    # Remove a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is true, else, it is false.
    def out(key)
      # (native code)
    end
    # Retrieve a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is the value of the corresponding record.  `nil' is returned if no record corresponds.
    def get(key)
      # (native code)
    end
    # Get the size of the value of a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is the size of the value of the corresponding record, else, it is -1.
    def vsiz(key)
      # (native code)
    end
    # Initialize the iterator.
    # If successful, the return value is true, else, it is false.
    # The iterator is used in order to access the key of every record stored in a database.
    def iterinit()
      # (native code)
    end
    # Get the next key of the iterator.
    # If successful, the return value is the next key, else, it is `nil'.  `nil' is returned when no record is to be get out of the iterator.
    # It is possible to access every record by iteration of calling this function.  It is allowed to update or remove records whose keys are fetched while the iteration.  However, it is not assured if updating the database is occurred while the iteration.  Besides, the order of this traversal access method is arbitrary, so it is not assured that the order of storing matches the one of the traversal access.
    def iternext()
      # (native code)
    end
    # Synchronize updated contents with the file and the device.
    # If successful, the return value is true, else, it is false.
    # This function is useful when another process connects the same database file.
    def sync()
      # (native code)
    end
    # Optimize the database file.
    # `<i>bnum</i>' specifies the number of elements of the bucket array.  If it is not defined or not more than 0, the default value is specified.  The default value is two times of the number of records.
    # `<i>apow</i>' specifies the size of record alignment by power of 2.  If it is not defined or negative, the current setting is not changed.
    # `<i>fpow</i>' specifies the maximum number of elements of the free block pool by power of 2.  If it is not defined or negative, the current setting is not changed.
    # `<i>opts</i>' specifies options by bitwise or: `TokyoCabinet::HDB::TLARGE' specifies that the size of the database can be larger than 2GB by using 64-bit bucket array, `TokyoCabinet::HDB::TDEFLATE' specifies that each record is compressed with Deflate encoding, `TokyoCabinet::HDB::TTCBS' specifies that each record is compressed with TCBS encoding.  If it is 0xff, the default setting is not changed.  If it is not defined, no option is specified.
    # If successful, the return value is true, else, it is false.
    # This function is useful to reduce the size of the database file with data fragmentation by successive updating.
    def optimize(bnum, apow, fpow, opts)
      # (native code)
    end
    # Remove all records.
    # If successful, the return value is true, else, it is false.
    def vanish()
      # (native code)
    end
    # Copy the database file.
    # `<i>path</i>' specifies the path of the destination file.  If it begins with `@', the trailing substring is executed as a command line.
    # If successful, the return value is true, else, it is false.  False is returned if the executed command returns non-zero code.
    # The database file is assured to be kept synchronized and not modified while the copying or executing operation is in progress.  So, this function is useful to create a backup file of the database file.
    def copy(path)
      # (native code)
    end
    # Get the path of the database file.
    # The return value is the path of the database file or `nil' if the object does not connect to any database file.
    def path()
      # (native code)
    end
    # Get the number of records.
    # The return value is the number of records or 0 if the object does not connect to any database file.
    def rnum()
      # (native code)
    end
    # Get the size of the database file.
    # The return value is the size of the database file or 0 if the object does not connect to any database file.
    def fsiz()
      # (native code)
    end
  end
  # B+ tree database is a file containing a B+ tree and is handled with the B+ tree database API.  Before operations to store or retrieve records, it is necessary to open a database file and connect the B+ tree database object to it.  To avoid data missing or corruption, it is important to close every database file when it is no longer in use.
  # Except for the interface below, methods compatible with the `Hash' class are also provided; `[]', `[]=', `store', `delete', `fetch', `has_key?', `has_value?', `key', `clear', `size', `empty?', `each', `each_key', `each_value', `keys', and `value'.
  class BDB
    # error code: success
    TCESUCCESS = 0
    # error code: threading error
    TCETHREAD = 1
    # error code: invalid operation
    TCEINVALID = 2
    # error code: file not found
    TCENOFILE = 3
    # error code: no permission
    TCENOPERM = 4
    # error code: invalid meta data
    TCEMETA = 5
    # error code: invalid record header
    TCERHEAD = 6
    # error code: open error
    TCEOPEN = 7
    # error code: close error
    TCECLOSE = 8
    # error code: trunc error
    TCETRUNC = 9
    # error code: sync error
    TCESYNC = 10
    # error code: stat error
    TCESTAT = 11
    # error code: seek error
    TCESEEK = 12
    # error code: read error
    TCEREAD = 13
    # error code: write error
    TCEWRITE = 14
    # error code: mmap error
    TCEMMAP = 15
    # error code: lock error
    TCELOCK = 16
    # error code: unlink error
    TCEUNLINK = 17
    # error code: rename error
    TCERENAME = 18
    # error code: mkdir error
    TCEMKDIR = 19
    # error code: rmdir error
    TCERMDIR = 20
    # error code: existing record
    TCEKEEP = 21
    # error code: no record found
    TCENOREC = 22
    # error code: miscellaneous error
    TCEMISC = 9999
    # tuning option: use 64-bit bucket array
    TLARGE = 1 << 0
    # tuning option: compress each record with Deflate
    TDEFLATE = 1 << 1
    # tuning option: compress each record with TCBS
    TTCBS = 1 << 2
    # open mode: open as a reader
    OREADER = 1 << 0
    # open mode: open as a writer
    OWRITER = 1 << 1
    # open mode: writer creating
    OCREAT = 1 << 2
    # open mode: writer truncating
    OTRUNC = 1 << 3
    # open mode: open without locking
    ONOLCK = 1 << 4
    # open mode: lock without blocking
    OLCKNB = 1 << 5
    # Create a B+ tree database object.
    # The return value is the new B+ tree database object.
    def initialize()
      # (native code)
    end
    # Get the message string corresponding to an error code.
    # `<i>ecode</i>' specifies the error code.
    # The return value is the message string of the error code.
    def errmsg(ecode)
      # (native code)
    end
    # Get the last happened error code.
    # The return value is the last happened error code.
    def ecode()
      # (native code)
    end
    # Set the tuning parameters.
    # `<i>lmemb</i>' specifies the number of members in each leaf page.  If it is not defined or not more than 0, the default value is specified.  The default value is 128.
    # `<i>nmemb</i>' specifies the number of members in each non-leaf page.  If it is not defined or not more than 0, the default value is specified.  The default value is 256.
    # `<i>bnum</i>' specifies the number of elements of the bucket array.  If it is not defined or not more than 0, the default value is specified.  The default value is 16381.  Suggested size of the bucket array is about from 0.5 to 4 times of the number of all records to be stored.
    # `<i>apow</i>' specifies the size of record alignment by power of 2.  If it is not defined or negative, the default value is specified.  The default value is 4 standing for 2^4=16.
    # `<i>fpow</i>' specifies the maximum number of elements of the free block pool by power of 2.  If it is not defined or negative, the default value is specified.  The default value is 10 standing for 2^10=1024.
    # `<i>opts</i>' specifies options by bitwise or: `TokyoCabinet::BDB::TLARGE' specifies that the size of the database can be larger than 2GB by using 64-bit bucket array, `TokyoCabinet::BDB::TDEFLATE' specifies that each record is compressed with Deflate encoding, `TokyoCabinet::BDB::TTCBS' specifies that each record is compressed with TCBS encoding.  If it is not defined, no option is specified.
    # If successful, the return value is true, else, it is false.  Note that the tuning parameters of the database should be set before the database is opened.
    def tune(lmemb, nmemb, bnum, apow, fpow, opts)
      # (native code)
    end
    # Set the caching parameters.
    # `<i>lcnum</i>' specifies the maximum number of leaf nodes to be cached.  If it is not defined or not more than 0, the default value is specified.
    # `<i>ncnum</i>' specifies the maximum number of non-leaf nodes to be cached.  If it is not defined or not more than 0, the default value is specified.
    # If successful, the return value is true, else, it is false.
    # Note that the tuning parameters of the database should be set before the database is opened.
    def setcache(lcnum, ncnum)
      # (native code)
    end
    # Open a database file.
    # `<i>path</i>' specifies the path of the database file.
    # `<i>omode</i>' specifies the connection mode: `TokyoCabinet::BDB::OWRITER' as a writer, `TokyoCabinet::BDB::OREADER' as a reader.  If the mode is `TokyoCabinet::BDB::OWRITER', the following may be added by bitwise or: `TokyoCabinet::BDB::OCREAT', which means it creates a new database if not exist, `TokyoCabinet::BDB::OTRUNC', which means it creates a new database regardless if one exists.  Both of `TokyoCabinet::BDB::OREADER' and `TokyoCabinet::BDB::OWRITER' can be added to by bitwise or: `TokyoCabinet::BDB::ONOLCK', which means it opens the database file without file locking, or `TokyoCabinet::BDB::OLCKNB', which means locking is performed without blocking.  If it is not defined, `TokyoCabinet::BDB::OREADER' is specified.
    # If successful, the return value is true, else, it is false.
    def open(path, omode)
      # (native code)
    end
    # Close the database file.
    # If successful, the return value is true, else, it is false.
    # Update of a database is assured to be written when the database is closed.  If a writer opens a database but does not close it appropriately, the database will be broken.
    def close()
      # (native code)
    end
    # Store a record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, it is overwritten.
    def put(key, value)
      # (native code)
    end
    # Store a new record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, this function has no effect.
    def putkeep(key, value)
      # (native code)
    end
    # Concatenate a value at the end of the existing record.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If there is no corresponding record, a new record is created.
    def putcat(key, value)
      # (native code)
    end
    # Store a record with allowing duplication of keys.
    # `<i>key</i>' specifies the key.
    # `<i>value</i>' specifies the value.
    # If successful, the return value is true, else, it is false.
    # If a record with the same key exists in the database, the new record is placed after the existing one.
    def putdup(key, value)
      # (native code)
    end
    # Remove a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is true, else, it is false.
    def out(key)
      # (native code)
    end
    # Retrieve a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is the value of the corresponding record.  `nil' is returned if no record corresponds.
    def get(key)
      # (native code)
    end
    # Retrieve a records.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is a list object of the values of the corresponding records.  `nil' is returned if no record corresponds.
    def getlist(key)
      # (native code)
    end
    # Get the number of records corresponding a key.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is the number of the corresponding records, else, it is 0.
    def vnum(key)
      # (native code)
    end
    # Get the size of the value of a record.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is the size of the value of the corresponding record, else, it is -1.
    def vsiz(key)
      # (native code)
    end
    # Synchronize updated contents with the file and the device.
    # If successful, the return value is true, else, it is false.
    # This function is useful when another process connects the same database file.
    def sync()
      # (native code)
    end
    # Optimize the database file.
    # `<i>lmemb</i>' specifies the number of members in each leaf page.  If it is not defined or not more than 0, the default value is specified.  The default value is 128.
    # `<i>nmemb</i>' specifies the number of members in each non-leaf page.  If it is not defined or not more than 0, the default value is specified.  The default value is 256.
    # `<i>bnum</i>' specifies the number of elements of the bucket array.  If it is not defined or not more than 0, the default value is specified.  The default value is two times of the number of records.
    # `<i>apow</i>' specifies the size of record alignment by power of 2.  If it is not defined or negative, the current setting is not changed.
    # `<i>fpow</i>' specifies the maximum number of elements of the free block pool by power of 2.  If it is not defined or negative, the current setting is not changed.
    # `<i>opts</i>' specifies options by bitwise or: `TokyoCabinet::BDB::TLARGE' specifies that the size of the database can be larger than 2GB by using 64-bit bucket array, `TokyoCabinet::BDB::TDEFLATE' specifies that each record is compressed with Deflate encoding, `TokyoCabinet::BDB::TTCBS' specifies that each record is compressed with TCBS encoding.  If it is 0xff, the default setting is not changed.  If it is not defined, no option is specified.
    # If successful, the return value is true, else, it is false.
    # This function is useful to reduce the size of the database file with data fragmentation by successive updating.
    def optimize(lmemb, nmemb, bnum, apow, fpow, opts)
      # (native code)
    end
    # Remove all records.
    # If successful, the return value is true, else, it is false.
    def vanish()
      # (native code)
    end
    # Copy the database file.
    # `<i>path</i>' specifies the path of the destination file.  If it begins with `@', the trailing substring is executed as a command line.
    # If successful, the return value is true, else, it is false.  False is returned if the executed command returns non-zero code.
    # The database file is assured to be kept synchronized and not modified while the copying or executing operation is in progress.  So, this function is useful to create a backup file of the database file.
    def copy(path)
      # (native code)
    end
    # Begin the transaction.
    # If successful, the return value is true, else, it is false.
    # The database is locked by the thread while the transaction so that only one transaction can be activated with a database object at the same time.  Thus, the serializable isolation level is assumed if every database operation is performed in the transaction.
    def tranbegin()
      # (native code)
    end
    # Commit the transaction of a B+ tree database object.
    # If successful, the return value is true, else, it is false.
    # Update in the transaction is fixed when it is committed successfully.
    def trancommit()
      # (native code)
    end
    # Abort the transaction of a B+ tree database object.
    # If successful, the return value is true, else, it is false.
    # Update in the transaction is discarded when it is aborted.  The state of the database is rollbacked to before transaction.
    def tranabort()
      # (native code)
    end
    # Get the path of the database file.
    # The return value is the path of the database file or `nil' if the object does not connect to any database file.
    def path()
      # (native code)
    end
    # Get the number of records.
    # The return value is the number of records or 0 if the object does not connect to any database file.
    def rnum()
      # (native code)
    end
    # Get the size of the database file.
    # The return value is the size of the database file or 0 if the object does not connect to any database file.
    def fsiz()
      # (native code)
    end
  end
  # Cursor is a mechanism to access each record of B+ tree database in ascending or descending order.
  class BDBCUR
    # cursor put mode: current
    CPCURRENT = 0
    # cursor put mode: before
    CPBEFORE = 1
    # cursor put mode: after
    CPAFTER = 2
    # Create a cursor object.
    # `<i>bdb</i>' specifies the B+ tree database object.
    # The return value is the new cursor object.
    # Note that the cursor is available only after initialization with the `first' or the `jump' functions and so on.  Moreover, the position of the cursor will be indefinite when the database is updated after the initialization of the cursor.
    def initialize(bdb)
      # (native code)
    end
    # Move the cursor to the first record.
    # If successful, the return value is true, else, it is false.  False is returned if there is no record in the database.
    def first()
      # (native code)
    end
    # Move the cursor to the last record.
    # If successful, the return value is true, else, it is false.  False is returned if there is no record in the database.
    def last()
      # (native code)
    end
    # Move the cursor to the front of records corresponding a key.
    # `<i>key</i>' specifies the key.
    # If successful, the return value is true, else, it is false.  False is returned if there is no record corresponding the condition.
    # The cursor is set to the first record corresponding the key or the next substitute if completely matching record does not exist.
    def jump(key)
      # (native code)
    end
    # Move the cursor to the previous record.
    # If successful, the return value is true, else, it is false.  False is returned if there is no previous record.
    def prev()
      # (native code)
    end
    # Move the cursor to the next record.
    # If successful, the return value is true, else, it is false.  False is returned if there is no next record.
    def next()
      # (native code)
    end
    # Insert a record around the cursor object.
    # `<i>value</i>' specifies the value.
    # `<i>cpmode</i>' specifies detail adjustment: `TokyoCabinet::BDBCUR::CPCURRENT', which means that the value of the current record is overwritten, `TokyoCabinet::BDBCUR::CPBEFORE', which means that the new record is inserted before the current record, `TokyoCabinet::BDBCUR::CPAFTER', which means that the new record is inserted after the current record.
    # If successful, the return value is true, else, it is false.  False is returned when the cursor is at invalid position.
    # After insertion, the cursor is moved to the inserted record.
    def put(value, cpmode)
      # (native code)
    end
    # Delete the record where the cursor is.
    # If successful, the return value is true, else, it is false.  False is returned when the cursor is at invalid position.
    # After deletion, the cursor is moved to the next record if possible.
    def out()
      # (native code)
    end
    # Get the key of the record where the cursor is.
    # If successful, the return value is the key, else, it is `nil'.  'nil' is returned when the cursor is at invalid position.
    def key()
      # (native code)
    end
    # Get the value of the record where the cursor is.
    # If successful, the return value is the value, else, it is `nil'.  'nil' is returned when the cursor is at invalid position.
    def val()
      # (native code)
    end
  end
end
