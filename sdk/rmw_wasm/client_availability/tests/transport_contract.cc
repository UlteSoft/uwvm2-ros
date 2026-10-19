/* SPDX-License-Identifier: Apache-2.0
 * UNEXECUTED local synthetic raw12 models; actual CPP10 generated objects and
 * the original complete service model fixture are reused, not Native proof.
 * The original18-group entrypoint is renamed and is NOT called by this unit.
 */
#define main original_service_model_entrypoint_not_invoked
#include "../../service_transport/tests/model_contract.cc"
#undef main
int main(){uwvm_rmw_cpp::Registry registry{cpp_fixture::published(),128};if(registry.error())return(int)registry.error();auto add=registry.codec(uwvm_rmw_cpp::Codec::add_service);if(!add)return 1;
 {model::Fixture f{add};uwvm_rmw_c_availability_provider out{};CHECK(!uwvm_rmw_service_transport_availability_callbacks(f.state,&out)&&out.version==1&&out.state==f.state&&out.query);unsigned answer=99;
 CHECK(!out.query(out.state,&f.native,&f.source,&f.availability,&answer)&&answer==1&&f.h.available_calls==1&&!f.h.submits);
 f.h.available_error=13;answer=99;CHECK(out.query(out.state,&f.native,&f.source,&f.availability,&answer)==13&&answer==99&&f.h.available_calls==2);
 f.h.available_error=125;CHECK(out.query(out.state,&f.native,&f.source,&f.availability,&answer)==125&&answer==99);
 f.h.available_error=0;f.h.invalid_boolean=true;CHECK(out.query(out.state,&f.native,&f.source,&f.availability,&answer)==22&&answer==99);
 auto wrong=f.native;++wrong.native_incarnation;unsigned before=f.h.available_calls;CHECK(out.query(out.state,&wrong,&f.source,&f.availability,&answer)==13&&answer==99&&f.h.available_calls==before);
 roswasmc_context other{&f.backend};auto foreign=f.availability;foreign.context=&other;CHECK(out.query(out.state,&f.native,&f.source,&foreign,&answer)==13&&answer==99&&f.h.available_calls==before);
 CHECK(out.query(out.state,&f.native,&f.source,&f.source,&answer)==13&&answer==99);CHECK(out.query(out.state,&f.native,&f.source,&f.peer,&answer)==13&&answer==99);
 CHECK(uwvm_rmw_service_transport_availability_callbacks(f.state,nullptr)==22);CHECK(uwvm_rmw_service_transport_dispose_unpublished(&f.state)==17);
 }
 std::printf("%s_ORIGINAL_RMW_AVAILABILITY_TRANSPORT_MODEL checks=%u failures=%u native=0\n",model::failures?"FAIL":"PASS",model::checks,model::failures);return model::failures?1:0;}
