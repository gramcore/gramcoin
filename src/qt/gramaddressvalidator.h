// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GRAM_QT_GRAMADDRESSVALIDATOR_H
#define GRAM_QT_GRAMADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class GRAMAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GRAMAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** GRAM address widget validator, checks for a valid gram address.
 */
class GRAMAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GRAMAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // GRAM_QT_GRAMADDRESSVALIDATOR_H
