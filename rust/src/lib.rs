#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("plugin.hpp");

        type BlobstoreClient;

        fn new_blobstore_client() -> UniquePtr<BlobstoreClient>;
    }
}
