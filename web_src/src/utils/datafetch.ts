import exp from 'constants'
import { http } from './http'

export const login = async (username: string, password: string) => {
  const res = await http.post<{
    token: string
  }>('/user/login', {
    username,
    password,
  })
  return res.data
}

export const logout = async () => {
  const res = await http.get('/user/logout')
  return res.status === 200
}

export const register = async (username: string, password: string) => {
  const res = await http.post<{ token: string }>('/user/register', {
    username,
    password,
  })
  return res.data
}

export interface UserInfoProps {
  username: string
  balance: number
  lastDealTime: number
  interest: number
}

export const getProfileInfo = async () => {
  const res = await http.get<UserInfoProps>('/user/info')
  return res.data
}

export const moneyDeal = async (money: number, toUserId?: string) => {
  const res = await http.post<{
    code: number
  }>('/user/money_deal', {
    money,
    toUserId,
  })
  return res.data.code === 0
}

export const saveMoney = async (money: number) => {
  const res = await http.post<{
    code: number
    balance: number
  }>('/user/save_money', {
    money,
  })
  return res.data.code === 0
}

export const withdrawMoney = async (money: number) => {
  const res = await http.post<{
    code: number
    balance: number
  }>('/user/withdraw_money', {
    money,
  })
  return res.data.code === 0
}

export interface RecordProps {
  id: string
  fromUserId: string
  toUserId?: string
  money: number
  dealTime: number
  comment?: string
}

export const getRecordList = async () => {
  const res = await http.get<{ list: RecordProps[] }>('/record/list')
  return res.data.list
}

export interface SummaryProps {
  income: number
  expend: number
}

export const getThisMonthSummary = async () => {
  const res = await http.get<SummaryProps>('/record/this_month_summary')
  return res.data
}
