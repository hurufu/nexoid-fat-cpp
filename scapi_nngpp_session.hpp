#pragma once

#include "scapi_session.hpp"
#include <experimental/propagate_const>

namespace scapi::nngpp {

    class Session : public ::scapi::Session {
        struct Impl;
        ::std::experimental::propagate_const<::std::unique_ptr<Impl>> pimpl;
    public:
        Session(void);
        ~Session(void) noexcept override;

        ::scapi::Response
        interaction(const ::scapi::Request&, std::chrono::milliseconds) override;

        ::scapi::Notification
        notification(std::chrono::milliseconds) override;

        std::vector<CandidateApplication>
        build_candidate_list(const ::scapi::BuildCandidateList&, const std::chrono::milliseconds) override;
    };

}
