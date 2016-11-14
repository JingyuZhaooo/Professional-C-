#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> mOps;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) const = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int mvalue;
	virtual void CodeGen(CodeContext& context) const override { }
};

class NStatement : public Node
{
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NBoolean : public Node
{
};

class NBlock : public Node
{
public:
	NBlock() { mbMainBlock = false; }
	virtual void CodeGen(CodeContext& context) const override;
	void AddStatement(NStatement* statement);
	void SetMainBlock() { mbMainBlock = true; }
private:
	std::list<NStatement*> mStatements;
	bool mbMainBlock;
};

class NForward : public NStatement
{
public:
	NForward() {};
	virtual void CodeGen(CodeContext& context) const override;
};


class NIs_Zombie : public NBoolean
{
public:
	NIs_Zombie(NNumeric* num) { mNum = num; };
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mNum;
};

class NIs_Human : public NBoolean
{
public:
	NIs_Human(NNumeric* num) { mNum = num; };
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mNum;
};

class NIs_Passable : public NBoolean
{
public:
	virtual void CodeGen(CodeContext& context) const override;
};

class NIs_random : public NBoolean
{
public:
	virtual void CodeGen(CodeContext& context) const override;
};

class NAttack : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context) const override;
};

class NRanged_Attack : public NStatement
{
public:
	virtual void CodeGen(CodeContext& context) const override;
};