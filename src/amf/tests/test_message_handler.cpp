#include "lest/lest.hpp"
#include "uau/handlerset.h"


class HandlerName {
public:
    HandlerName() :
        isObserver(true) {}

    HandlerName(const std::string &name) {
        const char *sep = handlerName.empty() ? "" : ";";

        handlerName.append(sep).append(name);
    }

    std::string name() const {
        return handlerName;
    }

    ~HandlerName() {
        if(isObserver)
            handlerName.clear();
    }

private:
    static std::string handlerName;
    bool isObserver = false;
};

std::string HandlerName::handlerName;


class Message {
public:
    virtual ~Message() {}
};

class Msg1 : public Message{};
class Msg2 : public Message{};
class Msg3 : public Message{};
class Msg4 : public Message{};
class Msg5 : public Message{};


void handlerFreeFunc() {
    HandlerName("handlerFreeFunc");
}


void handlerFreeFuncWithArgs(int i, const std::string &str) {
    HandlerName("handlerFreeFuncWithArgs");
}

class Foo {
public:
    void bar(int) const {
        HandlerName("Foo::bar");
    }
};

void handlerForMultipleMessages() {
    HandlerName("handlerForMultipleMessages");
}

uau::HandlerSet<Message> handler;

const lest::test specification[] = {
    "Msg1 -> handlerFreeFunc()", []{
        HandlerName lastInvokedHandler;

        std::unique_ptr<Message> msg(new Msg1);

        EXPECT(handler.handle(msg.get()));
        EXPECT(lastInvokedHandler.name() == "handlerFreeFunc");
    },

    "Msg2 -> handlerFreeFuncWithArgs(int i, const std::string &str)",[]{
        HandlerName lastInvokedHandler;
        std::unique_ptr<Message> msg(new Msg2);

        EXPECT(handler.handle(msg.get()));
        EXPECT(lastInvokedHandler.name() == "handlerFreeFuncWithArgs");
    },

    "Msg3 -> Foo::bar(int)",[]{
        HandlerName lastInvokedHandler;
        std::unique_ptr<Message> msg(new Msg3);

        EXPECT(handler.handle(msg.get()));
        EXPECT(lastInvokedHandler.name() == "Foo::bar");
    },

    "Msg4 -> handlerForMultipleMessages()",[]{
        HandlerName lastInvokedHandler;
        std::unique_ptr<Message> msg(new Msg4);

        EXPECT(handler.handle(msg.get()));
        EXPECT(lastInvokedHandler.name() == "handlerForMultipleMessages");
    },

    "Msg5 -> handlerForMultipleMessages()",[]{
        HandlerName lastInvokedHandler;
        std::unique_ptr<Message> msg(new Msg5);

        EXPECT(handler.handle(msg.get()));
        EXPECT(lastInvokedHandler.name() == "handlerForMultipleMessages");
    },

    "move constructor", []{
        HandlerName lastInvokedHandler;

        uau::HandlerSet<Message> h1;
        h1.setHandlerFor<Msg1>([]{});

        std::unique_ptr<Message> msg(new Msg1);
        EXPECT(h1.handle(msg.get()));

        uau::HandlerSet<Message> h2 = std::move(h1);
        EXPECT(!h1.handle(msg.get()));
        EXPECT(h2.handle(msg.get()));

        h1.setHandlerFor<Msg3>([]{});
        std::unique_ptr<Message> msg3(new Msg3);
        EXPECT(h1.handle(msg3.get()));
        EXPECT(!h2.handle(msg3.get()));
    },

    "move assignment", []{
        HandlerName lastInvokedHandler;

        uau::HandlerSet<Message> h1;
        h1.setHandlerFor<Msg1>(handlerFreeFunc);

        std::unique_ptr<Message> msg(new Msg1);
        EXPECT(h1.handle(msg.get()));

        uau::HandlerSet<Message> h2;
        h2 = std::move(h1);
        EXPECT(!h1.handle(msg.get()));
        EXPECT(h2.handle(msg.get()));

        h1.setHandlerFor<Msg3>([]{});
        std::unique_ptr<Message> msg3(new Msg3);
        EXPECT(h1.handle(msg3.get()));
        EXPECT(!h2.handle(msg3.get()));
    },

    "handlers overriding", []{
        uau::HandlerSet<Message> h;
        std::unique_ptr<Message> msg(new Msg1);

        {
            HandlerName lastInvokedHandler;
            h.setHandlerFor<Msg1>(handlerFreeFunc);

            EXPECT(h.handle(msg.get()));
            EXPECT(lastInvokedHandler.name() == "handlerFreeFunc");
        }

        {
            HandlerName lastInvokedHandler;
            h.setHandlerFor<Msg1>(handlerForMultipleMessages);
            EXPECT(h.handle(msg.get()));
            EXPECT(lastInvokedHandler.name() == "handlerForMultipleMessages");
        }
    },

    "message passing", [] {
        uau::HandlerSet<Message> actions;

        actions.setHandlerFor<Msg1>([](const Message *b){
            EXPECT(dynamic_cast<const Msg1 *>(b));
        }, std::placeholders::_1);

        std::unique_ptr<Message> msg(new Msg1);
        EXPECT(actions.handle(msg.get()));

        msg.reset(new Msg2);
        EXPECT(!actions.handle(msg.get()));
    }
};

int main() {
    handler.setHandlerFor<Msg1>(handlerFreeFunc);
    handler.setHandlerFor<Msg2>(handlerFreeFuncWithArgs, 1, "second");
    Foo foo;
    handler.setHandlerFor<Msg3>(&Foo::bar, &foo, 1);
    handler.setHandlerFor<Msg4, Msg5>(handlerForMultipleMessages);

    return lest::run(specification);
}
